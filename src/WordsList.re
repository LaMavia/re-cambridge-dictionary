open Helpers;
open Helpers.Reactive;

[@react.component]
let make = (~words: array(word)) => {
  <ol className="words" id="words">
    {words->renderArray((i: int, w: word) =>
       <li className="words__item" key={i |> string_of_int}>
         <h2 className="words__item__word"> {w.word |> React.string} </h2>
         <ol className="words__item__defs">
           {w.defs
            ->renderArray((j, d) =>
                <li
                  className="words__item__defs__item" key={j |> string_of_int}>
                  <p className="words__item__defs__item__def">
                    {(j + 1 |> string_of_int) ++ ". " ++ d.def |> React.string}
                  </p>
                  <p className="words__item__defs__item__eg">
                    {"Example: " ++ d.eg |> React.string}
                  </p>
                </li>
              )}
         </ol>
       </li>
     )}
  </ol>;
};