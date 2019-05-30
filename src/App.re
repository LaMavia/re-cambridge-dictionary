open Helpers;

[@react.component]
let make = (~initialWords: string) => {
  let (list: array(word), setList) =
    Helpers.useState([|
      {
        word: "mnemonic",
        defs: [|
          {
            def: "Something such as a very short poem or a special word used to help a person remember something",
            eg: "The musical notes on the lines go EGBDF - use the mnemonic \"Every good boy deserves fun\".",
          },
          {
            def: "Helping you to remember something",
            eg: "He uses mnemonic devices to help him remember his parking spot.The most effective programs teach mnemonic strategies",
          },
        |],
      },
    |]);
  let onReceived = (words: array(word)) => words |> setList;
  <section className="main">
    <WordsInput initialWords onReceived />
    <WordsList words=list />
  </section>;
};