# Grammar for sequences of notes

```
note_name ::= "c" | "d" | "e" | "f" | "g" | "a" | "b" ;
rest ::= "r" ;
alteration ::= "#" | "b" ;
octave ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" ;
duration_note ::= "w" | "h" | "q" | "e" | "s" | "t" | "x" ;
dot ::= "." ;
triplet ::= "3" ;
duration ::= duration_note
             | duration_note triplet
             | duration_note dot
             | duration_note dot triplet ;

octave_duration ::= octave
                    | octave duration ;

note ::= note_name
         | note_name octave_duration
         | rest
         | rest duration ;

sequence_of_notes ::= note
                      | note " " sequence_of_notes ;
```

# Grammar for envelopes

Example:
  0 +10:10 +2:100

```
zero ::= "0" ;
positive_digit ::= "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
digit ::= zero | positive_digit ;
digits ::= digit | digit digits ;
non_negative_integer ::= positive_digit digits | digit ;
positive_integer ::= positive_digit | positive_digit digits ;
envelope_term ::= "+" positive_integer ":" non_negative_integer ;
envelope_terms ::= envelope_term | envelope_term " " envelope_terms ;
evelope ::= non_negative_integer | non_negative_integer envelope_terms ;
```
