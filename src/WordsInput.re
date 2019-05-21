type state = {
  limit: int,
  inpt: string,
};

let fetchWords =
    (setState: array(Helpers.word) => unit, state: state)
    : Js.Promise.t(unit) => {
  let {inpt, limit} = state;
  Js.log(state);
  let arr = inpt |> Js.String.split(",") |> Js_json.stringArray;
  let base = Helpers.apiLink();
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "words", arr);

  Js.Promise.(
    {j|$base/?limit=$limit|j}
    ->Fetch.fetchWithInit(
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=
            payload
            |> Js_json.object_
            |> Js_json.stringify
            |> Fetch.BodyInit.make,
          (),
        ),
      )
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Helpers.parseWordsArray |> resolve)
    |> then_(ws => ws |> setState |> resolve)
  );
};

[@react.component]
let make = (~onReceived) => {
  let (state, setState) = Helpers.useState({limit: 1, inpt: ""});
  let onSubmit: Helpers.formListener =
    evt => {
      evt |> ReactEvent.Form.preventDefault;
      state |> fetchWords(onReceived);
      ();
    };

  <form className="form" onSubmit>
    <input
      className="form__input"
      onChange={evt => {
        evt->ReactEvent.Form.target##value
        ->(x => setState({...state, inpt: x}));
        ();
      }}
      type_="text"
      value={state.inpt}
      placeholder="You, know, nothing, Jon Snow"
    />
    <input
      className="form__input form__input--thin"
      type_="number"
      min=1
      max="9"
      value={state.limit |> string_of_int}
      onChange={evt => {
        evt->ReactEvent.Form.target##value
        ->(x => setState({...state, limit: x}));
        ();
      }}
    />
    <input type_="submit" value="Get Words!" />
  </form>;
};