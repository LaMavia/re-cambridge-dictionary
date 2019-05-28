type formListener = ReactEvent.Form.t => unit;
type stateUpdater('a) = 'a => unit;

type definition = {
  def: string,
  eg: string,
};
type word = {
  word: string,
  defs: array(definition),
};

[@bs.val] [@bs.scope ("window", "location")] external port: string = "";
[@bs.val] [@bs.scope ("window", "location")] external origin: string = "";

let parseWordDef = json =>
  Json.Decode.{
    def: json |> field("def", string),
    eg: json |> field("eg", string),
  };

let parseWord = json =>
  Json.Decode.{
    word: json |> field("word", string),
    defs: json |> field("defs", array(parseWordDef)),
  };

let parseWordsArray = json => json |> Json.Decode.array(parseWord);

let apiLink = (): string =>
  switch (port) {
  | "" => origin ++ "/api"
  | _ => "http://localhost:8080/api"
  };

let useState = initial => {
  React.useReducer((_ignored, newState) => newState, initial);
};

module Reactive = {
  let renderArray = (items: array('a), mapper: (int, 'a) => 'b) =>
    items->Belt.Array.mapWithIndex(mapper)->React.array;
};