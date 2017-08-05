let bpm = 250.
let reference_duration = 0.25
let sample_rate = 44100.
let master_volume = 0.5

let pi = 4. *. atan 1.

let freq note octave =
let p = ( octave *. 12. +. note -. 57. ) /. 12. in
2. ** p *. 440.

let c4_freq = freq 0.0 4.0

let rec explode s = match s with
  | "" -> []
  | s -> (String.get s 0 ) :: (explode (String.sub s 1 ( (String.length s)-1) ) )

let float_of_note s =
  let error = Invalid_argument ( "Invalid note `" ^ s ^ "`" ) in
  let accent lst = match lst with
    | [] -> 0.
    | '#' :: [] -> 1.
    | 'b' :: [] -> -1.
    | _ -> raise ( error ) in
  match explode s with
  | [] -> raise ( error )
  | c :: rest ->
    let a =  accent rest in
    ( match c with
      | 'c' -> 0.
      | 'd' -> 2.
      | 'e' -> 4.
      | 'f' -> 5.
      | 'g' -> 7.
      | 'a' -> 9.
      | 'b' -> 11.
      | 'p' -> ( match String.sub s 1 ( String.length s ) with
          | "" -> raise ( error )
          | s -> float_of_string ( String.sub s 1 (String.length s - 1) ) )
      | _ -> raise ( error ) ) +. a

let float_of_duration s =
  let error = Invalid_argument ( "Invalid duration `" ^ s ^ "`" ) in
  match explode s with
  | 'z' :: [] -> 16.
  | 'l' :: [] -> 8.
  | 'i' :: [] -> 4.
  | 'u' :: [] -> 2.
  | 'w' :: [] -> 1.
  | 'h' :: [] -> 0.5 (* 1/2 *)
  | 'q' :: [] -> 0.25 (* 1/4 *)
  | 'e' :: [] -> 0.125 (* 1/8 *)
  | 's' :: [] -> 0.0625 (* 1/16 *)
  | 't' :: [] -> 0.03125 (* 1/32 *)
  | 'x' :: [] -> 0.015625 (* 1/64 *)
  | 'd' :: rest -> ( match rest with
    | [] -> raise error
    | rest ->  0.015625 *. (
        float_of_string ( String.sub s 1 ( String.length s - 1 ) ) ) )
  | _ -> raise error

let note note octave =
  let note_f = float_of_note note in
  let octave_f = float octave in
  freq note_f octave_f

let bound lower upper x =
  if x < lower then lower
  else if x > upper then upper
  else x

let byte_of_sample sample =
  bound 0 255 ( int_of_float ( 255. *. ( ( 0.5 *. sample ) +. 0.5 ) ) )

let sine t =
  cos ( t *. 2. *. pi )

let square t =
  floor ( mod_float ( t *. 2. ) 2. ) *. 2. -. 1.

let saw t =
  mod_float t 1. *. 2. -. 1.

let triangle t =
  abs_float ( mod_float t 1. -. 0.5 )

let snd f freq = fun t -> f (freq *. t)

let dur d f = fun (t : float) -> if t < d then f t else 0.0

let scale a f = fun t -> a *. ( f t )

let scale_freq a f = fun t -> f ( a *. t )

let add f g = fun t -> ( f t ) +. ( g t )

let prod f g = fun t -> ( f t ) *. ( g t )

let prod_freq f g = fun t -> g ( ( f t ) *. t )

let delay d f = fun t -> f ( t -. d )

let cycle d f = fun t -> f ( mod_float t d )

let right_cut b f = fun t -> if t < b then f t else 0.0

let left_cut b f = fun t -> if t < b then 0.0 else f t

let cut a b f = fun t ->
if t < b then 0.0
else if t >= b then 0.0
else f t

let rec play f dt notes = match notes with
  | [] -> fun t -> None
  | ( d, freq ) :: tl -> fun t ->
    let a = 0.0 in
    if t < dt +. d -. a then Some (f ( freq *. t ))
    else if t < dt +. d then
      Some (( t -. dt -. d -. a  ) ** 2.0 /. a ** 2.0 *. ( f ( freq *. t ) ))
    else ( play f ( dt +. d ) tl ) t

(* Scalar product of a function : amplify the function *)
let ( *! ) a f = scale a f

(* Scalar product of two functions : volume envelope function *)
let ( *!@ ) f g = prod f g

(* Frequency product of a function : for frequency modulation *)
let ( *> ) a f = scale_freq a f

(* Frequency product of two function : frequency modulation with envelope function *)
let ( *>@ ) f g = prod_freq f g

(* Sum of two signals : signals are added in parallel *)
let ( +! ) f g = add f g

let () =
  let f = (1.0 /. c4_freq) *> (
          1.0 *! snd sine (note "c" 4) +!
          0.1 *! snd saw (note "f" 4) +!
          0.3 *! snd triangle (note "d#" 4) ) in
  let g = play f 0.0 [
      (0.25, note "a" 4);
      (0.25, note "g" 4);
      (0.5, note "f#" 4);
      (0.25, note "d" 3);
      (0.25, note "f" 3);
      (0.25, note "b" 3);
      (0.25, note "d" 4);
      (0.5, note "e" 4);
      (0.5, note "f" 3);
      (0.5, note "e" 3);
      (1.0, note "b" 2) ] in
  let rec loop t =
    let x = g (t /. sample_rate) in
    match x with
    | None -> ()
    | Some x ->
      output_byte stdout (byte_of_sample x);
      loop (t +. 1.) in
  loop 0.
