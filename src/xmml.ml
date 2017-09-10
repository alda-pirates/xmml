let bpm = 250.
let reference_duration = 0.25
let sample_rate = 44100.
let master_volume = 0.5

type 'a variant =
  | Const of 'a
  | Func of (float -> 'a)

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
  sin ( t *. 2. *. pi )

let pulse duty t =
  let t = mod_float (t -. 0.5) 1. in
  if t < duty then
    1.0
  else
    -1.0

let square t = pulse 0.5

let saw t =
  2. *. mod_float ( t -. 0.5 ) 1. -. 1.

let triangle t =
  4. *. abs_float ( mod_float ( t +. 0.25 ) 1. -. 0.5 ) -. 1.


  (* class  Monad m  where
      (>>=)            :: m a -> (a -> m b) -> m b
      (>>)             :: m a -> m b -> m b
      return           :: a -> m a
      fail             :: String -> m a

      m >> k           =  m >>= \_ -> k *)

let scale a f = fun t -> f t *. a

let add f g = fun t -> f t +. g t

let prod f g = fun t -> f t *. g t

let time_scale a f = fun t -> f ( t *. a )

let time_add f g = fun t -> g ( f t +. t )

let time_prod f g = fun t -> g ( f t *. t )

let time_comp f g = fun t -> g ( f t )

let delay shf f = fun t -> f ( t -. shf )

let cycle len f = fun t -> f ( mod_float t len )

let right_cut b f = fun t -> if t < b then f t else 0.0

let left_cut b f = fun t -> if t < b then 0.0 else f t

let cut a b f = fun t ->
if t < b then 0.0
else if t >= b then 0.0
else f t

(* Play notes using f as instrument *)
let play f notes =
  let rec play' f t0 notes =
    match notes with
    | [] -> fun t -> None
    | ( dur, freq, vol ) :: rest -> fun t ->
      if t < t0 +. dur then
        let a = f ( freq *. t ) in
        let env =
          if t < t0 +. 0.005 then
            ( t -. t0 ) /. 0.005
          else if t < t0 +. dur -. 0.005 then
            1.0
          else
            ( t0 +. dur -. t ) /. 0.005
        in
        Some  ( env *. vol *. a )
      else
        (play' f ( t0 +. dur ) rest ) t
  in
  play' f 0.0 notes

(* Scalar product of a function : amplify the function *)
let ( *< ) a f = scale a f

(* Frequency scaling of a function : amplify or attenuate the frequency with a scalar *)
let ( *> ) a f = time_scale a f

(* Sum of two signals : signals are added in parallel *)
let ( +<> ) f g = add f g

(* Product of two functions : volume envelope function *)
let ( *<> ) f g = prod f g

(* Frequency product of two functions : frequency modulation with envelope function *)
let ( *>> ) f g = time_prod f g

let () =
  let synth = (1.0 /. c4_freq) *> (
      1.0 *< (note "c" 4 *> sine) +<>
      0.5 *< (note "f" 4 *> saw)  +<>
      0.5 *< (note "d#" 4 *> triangle) ) in
  let flange = (fun t -> 0.6 *. pulse (triangle (t /. 600.) *. 0.4 +. 0.5) t) in
  let g = play flange [
      (0.2, note "a" 4, 1.0);
      (0.2, note "g" 4, 1.0);
      (0.4, note "f#" 4, 1.0);
      (0.2, note "d" 3, 1.0);
      (0.2, note "f" 3, 1.0);
      (0.2, note "b" 3, 1.0);
      (0.2, note "d" 4, 1.0);
      (0.4, note "e" 4, 1.0);
      (0.4, note "f" 3, 1.0);
      (0.4, note "e" 3, 1.0);
      (0.8, note "b" 2, 1.0) ] in
  let rec loop t =
    let x = g (t /. sample_rate) in
    match x with
    | None -> ()
    | Some x ->
      output_byte stdout (byte_of_sample x);
      loop (t +. 1.) in
  loop 0.
