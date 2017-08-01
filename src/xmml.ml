open TimeOp

let bpm = 250.
let reference_duration = 0.25
let sample_rate = 48000.
let master_volume = 0.5

let pi = 4. *. atan 1.

type note_letter = Note_letter of char
and note_accent = Sharp | Flat
and octave = Octave of int
and pitch = Pitch of note_letter * note_accent option * octave option
and duration_letter = Duration_letter of char
and dot = Dot
and triplet = Triplet
and duration = Duration of duration_letter * dot option * triplet option
and note = Note of pitch * duration option

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

let rec freq_of_pitch pitch last_octave =
  let freq note octave =
    let p = (octave *. 12. +. note -. 57.) /. 12. in
    2. ** p *. 440. in
  match pitch with
  | Pitch ( letter, accent, None ) ->
    freq_of_pitch
      ( Pitch ( letter, accent, Some ( Octave last_octave ) ) )
      last_octave
  | Pitch ( Note_letter c, None, Some ( Octave octave ) ) ->
    freq
      ( float_of_note ( String.make 1 c ) )
      ( float octave )
  | Pitch ( Note_letter c, Some Sharp, Some ( Octave octave ) ) ->
    freq
      ( float_of_note ( String.make 1 c ) +. 1. )
      ( float octave )
  | Pitch ( Note_letter c, Some Flat, Some ( Octave octave ) ) ->
    freq
      ( float_of_note ( String.make 1 c ) -. 1. )
      ( float octave )

let notes = [
  Note ( Pitch ( Note_letter 'c', None, Some ( Octave 4 ) ),
         Some ( Duration ( Duration_letter 'w', None, None )  ) );
  Note ( Pitch ( Note_letter 'd', None, None ), None );
  Note ( Pitch ( Note_letter 'e', None, None ), None );
  Note ( Pitch ( Note_letter 'f', None, None ), None );
  Note ( Pitch ( Note_letter 'g', None, None ), None );
  Note ( Pitch ( Note_letter 'a', None, None ), None );
  Note ( Pitch ( Note_letter 'b', None, None ), None ) ]

let freq note octave =
  let p = ( octave *. 12. +. note -. 57. ) /. 12. in
  2. ** p *. 440.

let c4_freq = freq 0.0 4.0

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

let float_sum lst =
  List.fold_left ( fun acc x -> x +. acc ) 0. lst

type signal = float -> float option

(* Scalar product of a signal : amplify the signal *)
let ( *! ) a f = fun ( t: float ) -> a *. f t

let ( *> ) a f = fun ( t: float ) -> f ( a *. t )

(* Sum of two signals : signals are added in parallel *)
let ( +! ) f g = fun ( t: float ) -> ( f t ) +. ( g t )

(* Concatenation of two signals : sinals are added sequentially *)
let ( ^! ) f (g, l) = fun ( t: float ) ->
  if t < l then f t
  else g (t -. l)

(* Takes a `float -> float` function and return a signal *)
let signal ( f: float -> float ) = fun t -> Some (f t)

(* TODO:
   A function `instrument: (freq: float) -> (time: float) -> float` *)
(* Base frequency of an instrument is c4 = 261.6256 *)
let instrument f  =
  let c4_freq = freq 0.0 4.0 in
  fun freq t -> f ( c4_freq /. freq *. t )

(* TODO:
   A function that turns "c4h d e f g a b" into
   a function `(time: float) -> float` *)

let snd f freq = fun t -> f (freq *. t)

let dur d f = fun (t : float) -> if t < d then f t else 0.0

let delay f d = fun t -> f ( t -. d )

let rec play f dt notes = match notes with
  | [] -> fun t -> 0.0
  | (d, freq) :: tl -> fun (t : float) ->
    if t < dt +. d then f (freq *. t)
    else (play f (dt +. d) tl) t

let () =
  let f = (1.0 /. c4_freq) *> (
          1.0 *! snd sine (note "c" 4) +!
          0.1 *! snd saw (note "a" 4) +!
          0.3 *! snd triangle (note "d#" 4) ) in
  let g = play f 0.0 [
      (0.25, note "c" 4);
      (0.25, note "d" 4);
      (0.5, note "e" 4);
      (0.25, note "f" 4);
      (0.25, note "g" 4);
      (0.5, note "a" 4);
      (0.5, note "b" 4) ] in
  let rec loop t =
    let x = g (t /. sample_rate) in
        output_byte stdout (byte_of_sample x);
        loop (t +. 1.) in
  loop 0.
