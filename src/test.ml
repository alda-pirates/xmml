type time = float
type 'a signal = time -> 'a
type audio_signal = float signal

(*
To eval a signal we need

a pitch: note_letter * accent * octave
a duration: dur_letter * dot * tuplet

let frequency note_letter accent octave =
let base_freq = match note_letter with
| 'c' -> 0.
| 'd' -> 2.
| 'e' -> 4.
| 'f' -> 5.
| 'g' -> 7.
| 'a' -> 9.
| 'b' -> 11.
| _ -> raise ( Invalid_argument ( "Invalid letter `"
                                  ^ ( String.make 1 c )
                                  ^ "` for note" ) ) in
let accent_mod = match accent with
| '#' -> 1.
| 'b' -> -1.
| _ -> raise ( Invalid_argument ( "Invalid accent `"
                                  ^ ( String.make 1 c )
                                  ^ "` for note" ) ) in

*)
