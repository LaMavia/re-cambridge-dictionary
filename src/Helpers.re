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

module Reactive = {
  let renderArray = (items: array('a), mapper: (int, 'a) => 'b) =>
    items->Belt.Array.mapWithIndex(mapper)->React.array;
};

module URL = {
  [@bs.val] [@bs.scope ("window", "location")] external port: string = "";
  [@bs.val] [@bs.scope ("window", "location")] external origin: string = "";
  [@bs.val] [@bs.scope ("window", "location")] external href: string = "";

  let setUrl: string => unit = [%bs.raw
    {|x => {
      const oldUrl = location.href;
      history.pushState(null, null, x);
      if(location.href === oldUrl) history.replaceState(null, null, x);
    }|}
  ];

  [@bs.val] external decodeURI: string => string = "";
  [@bs.val] external decodeURIComponent: string => string = "";
  [@bs.val] external encodeURI: string => string = "";
  [@bs.val] external encodeURIComponent: string => string = "";
};

[@bs.val] [@bs.scope ("window", "history")]
external pushState: (unit, unit, string) => unit = "";

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
  switch (URL.port) {
  | "" => URL.origin ++ "/api"
  | _ => "http://localhost:8080/api"
  };

let useState = initial => {
  React.useReducer((_ignored, newState) => newState, initial);
};