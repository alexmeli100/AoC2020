// Learn more about F# at http://docs.microsoft.com/dotnet/fsharp

open System
    
let getInput filename =
    let input = IO.File.ReadAllText filename
    
    input.Split([|"\n\n"|], StringSplitOptions.None)

let uniqueAnswers group =
    group
    |> Seq.filter(fun c -> c <> '\n')
    |> Set.ofSeq
    

let countYes groups =
    groups
    |> Seq.map(fun g -> Set.count <| uniqueAnswers g)
    |> Seq.reduce(fun a b -> a + b)
    
let countAnswered (answers: String[], a: char) =
    match answers |> Seq.forall(fun ans -> ans.Contains <| string a) with
        | true -> 1
        | _ -> 0
    
let countForGroup(group: String) =
    let answers = group.Split '\n'
    let maxAnswered = answers |> Seq.maxBy(fun x -> x.Length)
    
    maxAnswered
    |> Seq.map(fun c -> countAnswered(answers, c))
    |> Seq.reduce(fun a b -> a + b)

let countEvery(groups: String[]) =
    groups
    |> Seq.map(fun g -> countForGroup(g))
    |> Seq.reduce(fun a b -> a + b)
    

[<EntryPoint>]
let main argv =
    if argv.Length <> 1 then
        failwith "Error: Expected argument <input file>"
   
    let groups = getInput argv.[0]
    let part1 = countYes groups
    let part2 = countEvery(groups)
    printfn "Part1: %d" part1
    printfn "Part2: %d" part2
    0 