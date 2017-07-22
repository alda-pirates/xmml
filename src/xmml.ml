let bpm = 250.
let time_signature = (4, 4)
let sample_rate = 48000.
let master_volume = 0.5

let pi = 4. *. atan 1.

let freq note octave =
  let p = float (octave * 12 + note - 57) /. 12. in
  2. ** p *. 440.

type note_letter = Note_letter of char
and note_accent = Sharp | Flat
and octave = Octave of int
and pitch = Pitch of note_letter * note_accent option * octave option
and duration_letter = Duration_letter of char
and dot = Dot
and triplet = Triplet
and duration = Duration of duration_letter * dot option * triplet option
and note = Note of pitch * duration option

let int_of_note_letter c =
  match c with
  | 'c' -> 0
  | 'd' -> 2
  | 'e' -> 4
  | 'f' -> 5
  | 'g' -> 7
  | 'a' -> 9
  | 'b' -> 11
  | _ -> raise ( Invalid_argument ( "Invalid letter `"
                                    ^ ( String.make 1 c )
                                    ^ "` for note" ) )

let int_of_duration_letter c =
  match c with
  | 'w' -> 1
  | 'h' -> 2
  | 'q' -> 4
  | 'e' -> 8
  | 's' -> 16
  | 't' -> 32
  | 'x' -> 64
  | _ -> raise ( Invalid_argument ( "Invalid letter `"
                                    ^ ( String.make 1 c )
                                    ^ "` for duration" ) )

let rec freq_of_pitch pitch last_octave =
  match pitch with
    Pitch ( letter, accent, None )
    -> freq_of_pitch
         ( Pitch ( letter, accent,
                   Some ( Octave last_octave ) ) )
         last_octave
  | Pitch ( Note_letter c, None, Some ( Octave octave ) )
    -> freq ( int_of_note_letter c ) octave
  | Pitch ( Note_letter c, Some Sharp, Some ( Octave octave ) )
    -> freq ( ( int_of_note_letter c ) + 1 ) octave
  | Pitch ( Note_letter c, Some Flat, Some ( Octave octave ) )
    -> freq ( ( int_of_note_letter c ) - 1 ) octave

(* let float_of_duration dur last_duration =
  match dur with
    Duration ( Duration_letter letter, dot, _ )
    -> ( let dot_mult = ( match dot with None -> 1. | Some Dot -> 0.5 ) in
         let note_duration ) *)



let notes = [
  Note ( Pitch ( Note_letter 'c', None, Some ( Octave 4 ) ),
         Some ( Duration ( Duration_letter 'w', None, None )  ) );
  Note ( Pitch ( Note_letter 'd', None, None ), None );
  Note ( Pitch ( Note_letter 'e', None, None ), None );
  Note ( Pitch ( Note_letter 'f', None, None ), None );
  Note ( Pitch ( Note_letter 'g', None, None ), None );
  Note ( Pitch ( Note_letter 'a', None, None ), None );
  Note ( Pitch ( Note_letter 'b', None, None ), None ) ]

(* let float_list_of_note wav freq dur t =
  0

let rec float_list_notes wav notes =
  match notes with
  | [] -> []
  | note :: rest -> (float_list_of_note wav note 0) :: (float_list_notes wav rest)

let rec zip lst_a lst_b =
  match (lst_a, lst_b) with
    ([], []) -> []
  | (hd_a::tl_a, hd_b::tl_b) -> (hd_a, hd_b) :: zip tl_a tl_b
  | _ -> [] *)

let bound lower upper x =
  if x < lower then lower
  else if x > upper then upper
  else x

let byte_of_sample sample =
  bound 0 255 (int_of_float (255. *. ((0.5 *. sample) +. 0.5)))

let sine freq phase t =
  cos ((freq /. sample_rate *. t +. phase) *. 2. *. pi)

let square freq phase t =
  floor (mod_float (t /. sample_rate *. 2. *. freq) 2.) *. 2. -. 1.

let saw freq phase t =
  mod_float (t /. sample_rate *. freq) 1. *. 2. -. 1.

let triangle freq phase t =
  abs_float (mod_float (t /. sample_rate *. freq) 1. -. 0.5)

(* float_mod (t /. sample_rate *. freq) 1.  *)

let sum_signals lst =
  List.fold_left (fun acc x -> x +. acc) 0. lst

(* A function that turns "c4h d e f g a b"  *)

let () =
  let rec loop t =
    let r = master_volume *. sum_signals [
        1.  *. sine (freq 0 4) 0. t;
        0.1 *. saw (freq 7 4) 0. t;
        0.3 *. triangle (freq 4 4) 0. t
      ] in
    output_byte stdout (byte_of_sample r);
    (* Printf.printf "%.2f\n" (sine (freq 0 4) 0. t); *)
    loop (t +. 1.) in
  loop 0.
