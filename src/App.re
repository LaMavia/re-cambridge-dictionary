[@react.component]
let make = () => {
  let (list, setList) = Helpers.useState([||]);
  let onReceived = (words: array(Helpers.word)) => words |> setList;
  <section className="main">
    <WordsInput onReceived />
    <WordsList words=list />
  </section>
};