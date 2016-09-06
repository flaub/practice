open Batteries
open OUnit2

let qs_numbers test_ctxt = 
	let xs = [| 5; 2; 9; 43; 1; 4; 20; 99; 8 |] in
	Quicksort.qsort xs 0 ((Array.length xs) - 1);
	assert_equal
		~printer:(IO.to_string(Array.print Int.print)) 
		[| 1; 2; 4; 5; 8; 9; 20; 43; 99 |] xs
;;

let qs_words test_ctxt =
	let expected = DynArray.to_array (DynArray.of_enum (File.lines_of "sorted_words")) in
	let words = DynArray.to_array (DynArray.of_enum (File.lines_of "words")) in
	Quicksort.qsort words 0 ((Array.length words) - 1);
	(* Enum.iter (Printf.printf "%s\n") (words |> Array.enum |> (Enum.take 10)); *)
	assert_equal expected words
;;
 
let suite = 
	"quicksort" >::: [
		"numbers" >:: qs_numbers;
		"words"   >:: qs_words
	]
;;

let () =
	run_test_tt_main suite
;;
