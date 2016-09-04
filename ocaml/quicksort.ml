open Printf

let print_array label array =
	printf "%s\n" label;
	Array.iter (printf "%d ") array;
	printf "\n"

let swap array i j =
	let tmp = array.(!i) in
	array.(!i) <- array.(!j);
	array.(!j) <- tmp
;;

let rec qsort array first last =
	let len = last - first in
	let pivot = first + (len / 2) in
	let i = ref first in
	let j = ref (last - 1) in

	while !i <= !j do
		while array.(!i) < array.(pivot) do
			incr i;
		done;
		while array.(!j) > array.(pivot) do
			decr j;
		done;
		if !i <= !j then
			swap array i j;
			incr i;
			decr j;
	done;

	if first < !j then qsort array first !j;
	if !i < last then qsort array !i last;
;;

let main () =
	let test = [| 5; 2; 9; 43; 1; 4; 20; 99; 8 |] in
	let copy = Array.copy test in
	Array.sort compare copy;

	print_array "Pre" test;
	qsort test 0 (Array.length test);
	print_array "Post" test;
	print_array "Sorted" copy;
;;

main ()
