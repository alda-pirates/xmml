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

let rec list_car s = match s with
  | "" -> []
  | s -> (String.get s 0 ) :: (list_car (String.sub s 1 ( (String.length s)-1) ) )

let float_of_note s =
  let error = Invalid_argument ( "Invalid note `" ^ s ^ "`" ) in
  let accent lst = match lst with
    | [] -> 0.
    | '#' :: [] -> 1.
    | 'b' :: [] -> -1.
    | _ -> raise ( error ) in
  match list_car s with
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
  match list_car s with
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
let base_freq = float_of_note note in
let p = ( float octave *. 12. +. base_freq -. 57. ) /. 12. in
2. ** p *. 440.

let bound lower upper x =
  if x < lower then lower
  else if x > upper then upper
  else x

let byte_of_sample sample =
  bound 0 255 (int_of_float (255. *. ((0.5 *. sample) +. 0.5)))

let sine freq phase t =
  cos ((freq *. t +. phase) *. 2. *. pi)

let square freq phase t =
  floor (mod_float (t *. 2. *. freq) 2.) *. 2. -. 1.

let saw freq phase t =
  mod_float (t *. freq) 1. *. 2. -. 1.

let triangle freq phase t =
  abs_float (mod_float (t *. freq) 1. -. 0.5)

(* float_mod (t /. sample_rate *. freq) 1.  *)

let float_sum lst =
  List.fold_left (fun acc x -> x +. acc) 0. lst

type signal = ( float -> float option )

(* Scalar product of a signal : amplify the signal *)
let ( **$ ) s a = fun ( t: float ) -> match a t with
  | None -> None
  | Some x -> Some ( s *. x )

(* Product of two signals : signals are added in parallel *)
let ( *$ ) a b = fun ( t: float ) -> match (a t, b t) with
  | (None, None) -> None
  | (Some x, None) -> Some x
  | (None, Some x) -> Some x
  | (Some x, Some y) -> Some ( x +. y )

(* Sum of two signals : sinals are added sequentially *)
let ( +$ ) a b = ( fun ( t: float ) ->
  match a t with
  | None -> b t
  | x -> x )

let ( !$ ) a = ( fun t ->
  match a t with
  | None -> 0.
  | Some x -> x )

(* Takes a (float -> float) function and return a signal *)
let signal ( f: float -> float ) = ( fun t -> Some (f t) )

(* TODO: a function that takes a *)
(* TODO: a function that turns "c4h d e f g a b" into a function (float -> float) *)

let () =
  let f = 1.  **$ signal (sine (freq "c" 4) 0.) *$
          0.1 **$ signal (saw (freq "a" 4) 0.) *$
          0.3 **$ signal (triangle (freq "d#" 4) 0.) in
  let rec loop t =
    ( match f (t /. sample_rate) with
      | None -> ()
      | Some x ->
        output_byte stdout (byte_of_sample x);
        loop (t +. 1.) ) in
  loop 0.
