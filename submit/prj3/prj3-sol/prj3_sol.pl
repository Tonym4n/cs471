/************************** is_all_greater_than ************************/

%% #1 10-points
%is_all_greater_than(List, N): succeed iff number N is greater than
%all numbers in list of numbers List.
%%Hint: use recursion on List.

is_all_greater_than([], _).
is_all_greater_than([X|Xs], N) :-
	X > N,
	is_all_greater_than(Xs, N).

/*************************** get_greater_than1 *************************/

%% #2 10-points
%get_greater_than1(List, N, M): succeed iff M matches a number in List
%which is greater than N.
%
%Example:
% ?- get_greater_than1([2, 3, 1, 4], 2, M).
% M = 3 ;
% M = 4 ;
% false.
%
%Restriction: May not use any auxiliary procedures.
%Hint: Use separate rules for head and tail of List.

get_greater_than1([], _, _) :- 
	false.
get_greater_than1([X|Xs], N, M) :-
	X = M,
	M > N,
	true;
	get_greater_than1(Xs, N, M).

/*************************** get_greater_than2 *************************/

%% #3 10-points
%get_greater_than2(List, N, M): succeed iff M matches a number in List
%which is greater than N.  Same spec as previous exercise.
%Restriction: Must be a single rule which uses member/2.
%Hint: use generate-and-test with member/2 used to generate
%and comparison used to test.

get_greater_than2(List, N, M) :-
	member(M, List),
	M > N.

/************************* get_all_greater_than ************************/

%% #4 15-points
%get_all_greater_than(List, N, GreaterThanN): GreaterThanN matches
%the list of numbers from List (in order) which are greater than N.
%Restriction: May not use any auxiliary procedures.
%Hint: use a rule for empty List and two separate rules for a non-empty List.

get_all_greater_than([], _, []).
get_all_greater_than([X|Xs], N, GreaterThanN) :-
	X > N,
	get_all_greater_than(Xs, N, G),
	GreaterThanN = [X|G];
	X =< N,
	get_all_greater_than(Xs, N, G),
	GreaterThanN = G.

/*********************** get_all_greater_than_tr ***********************/

%% #5 15-points
%get_all_greater_than_tr(List, N, GreaterThanN): GreaterThanN matches
%the list of numbers from List (in order) which are greater than N.
%The procedure must be tail-recursive; i.e. the "return value" of
%any recursive calls must be the "return value" of the original call.
%Restriction: May define and use a *single* auxiliary procedure and
%either reverse/2 or append/3.
%Hint: Define an auxiliary procedure witn an extra argument which
%accumulates GreaterThanN.

get_all_greater_than_tr([], _, []).
get_all_greater_than_tr([X|Xs], N, [X|GreaterThanN]) :-
	X > N,
	get_all_greater_than_tr(Xs, N, GreaterThanN).
get_all_greater_than_tr([X|Xs], N, GreaterThanN) :-
	X =< N,
	get_all_greater_than_tr(Xs, N, GreaterThanN).

/*************************** split_into_pairs **************************/

%% #6 10-points
%split_into_pairs(List, PairList): PairList is a list of pairs (2-element
%lists) of elements of List in order.  If List has odd length, then the
%last element of PairList will match a 1-element list containing the last
%element of List.
%Restriction: May not use any auxiliary procedures.
%Hint: consider cases of List empty, List a 1-element list and List
%containing 2-or-more elements.

split_into_pairs([], []).
split_into_pairs([X], [[X]]).
split_into_pairs([X1, X2|Xs], [[X1, X2]|PairList]) :-
	split_into_pairs(Xs, PairList).

/******************************** sum_areas ****************************/
%% #7 10-points
%sum_areas(Shapes, SumArea): match SumArea to the sum of the shapes in
%list Shapes.  A shape is either rect(X, Y, W, H) with area W*H or
%circ(X, Y, R) with area pi * R * R (note pi is defined on the
%RHS of is/2).

sum_areas([], 0).
sum_areas([X|Xs], SumArea) :-
	X = rect(_, _, W, H),
	sum_areas(Xs, Areas),
	A is (W * H),
	SumArea is A + Areas;
	X = circ(_, _, R),
	sum_areas(Xs, Areas),
	A is pi * R * R,
	SumArea is A + Areas.

/******************************* n_prefix ******************************/

%% #8 10-points
%n_prefix(N, List, Prefix, Rest): Prefix matches the N-prefix of list List
%and Rest matches the rest of the list.  It is assumed that N > 0 and
%length(List) >= N.
%Restriction: May not use any auxiliary procedures.
%Hint: Recurse on N.

n_prefix(N, Xs, [], Xs) :-
	N = 0.
n_prefix(N, [X|Xs], [X|Prefix], Rest) :-
	N > 0,
	NMinusOne is N - 1,
	n_prefix(NMinusOne, Xs, Prefix, Rest).

/***************************** split_into_n_lists **********************/

%% #9 10-points
%split_into_n_lists(N, List, NLists): match NLists with the N-element
%sublists of List in order.  If length(List) is not divisible by N,
%leaving some leftover elements, then the last element of NLists will
%match the leftover elements of List.  It is assumed that N > 0.
%Hint: List is either empty or non-empty.  If non-empty, its
%length is < N, or its length is >= N; in the latter case, use n_prefix/3
%to recurse.

split_into_n_lists(_, [], []).
split_into_n_lists(N, Xs, [Prefix]) :-
	length(Xs, L),
	L < N,
	L > 0,
	n_prefix(L, Xs, Prefix, _).
split_into_n_lists(N, Xs, [Prefix|NLists]) :-
	length(Xs, L),
	L >= N,
	n_prefix(N, Xs, Prefix, Rest),
	split_into_n_lists(N, Rest, NLists).