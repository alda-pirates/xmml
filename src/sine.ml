let bpm = 250.
let sample_rate = 48000.
let master_volume = 0.5

let pi = 4. *. atan 1.

let freq note pitch =
  let p = float (pitch * 12 + note - 57) /. 12. in
  2. ** p *. 440.

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

let linear_envelope lst =

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
