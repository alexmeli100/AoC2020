-module(day1).

%% API exports
-export([main/1]).

%%====================================================================
%% API functions
%%====================================================================

%% escript Entry point
main(Path) ->
    solve(Path),

    erlang:halt(0).

%%====================================================================
%% Internal functions
%%====================================================================
solve(File) ->
  {ok, Data} = file:read_file(File),
  Nums = [begin {Int, _} = string:to_integer(Token), Int end|| Token <- binary:split(Data, [<<"\n">>], [global])],
  Combs2 = lib_combin:cnr(2, Nums),
  Combs3 = lib_combin:cnr(3, Nums),

  Sum = fun(X, Y) -> X + Y end,
  Mul = fun(X, Y) -> X * Y end,
  Pred = fun(L) -> lists:foldl(Sum, 0, L) == 2020 end,

  {value, L1} = lists:search(Pred, Combs2),
  {value, L2} = lists:search(Pred, Combs3),


  Part1 = lists:foldl(Mul, 1, L1),
  Part2 = lists:foldl(Mul, 1, L2),

  io:fwrite("Part1: ~w~n", [Part1]),
  io:fwrite("Part2: ~w~n", [Part2]).