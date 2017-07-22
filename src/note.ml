type note_char = Note_char of char
and octave = Octave of int
and duration_char = Duration_char of char
and dot = Dot
and triplet = Triplet
and duration = Duration of duration_char * dot option * triplet option
and note = Note of note_char * octave option * duration option

let notes = [
  Note (Note_char 'c', Some (Octave 4), Some (Duration (Duration_char 'w', None, None) ));
  Note (Note_char 'd', None, None);
  Note (Note_char 'e', None, None);
  Note (Note_char 'f', None, None);
  Note (Note_char 'g', None, None);
  Note (Note_char 'a', None, None);
  Note (Note_char 'b', None, None)
]


(* let eval_notes lst = *)
