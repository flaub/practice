open Batteries

let swap ar i j =
	let tmp = ar.(i) in
	ar.(i) <- ar.(j);
	ar.(j) <- tmp
;;

let partition ar lo hi =
	let pivot = ar.(hi) in
	let i = ref lo in

	for j = lo to (hi - 1) do
		if ar.(j) <= pivot then (
			swap ar !i j;
			incr i
		)
	done;
	swap ar !i hi;
	!i
;;

let rec qsort ar lo hi =
	if lo < hi then (
		let p = partition ar lo hi in
		qsort ar lo (p - 1);
		qsort ar (p + 1) hi;
	);
;;
