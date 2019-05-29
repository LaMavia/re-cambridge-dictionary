//@ts-check
const { scrape, format } = require('./cambridge')
/**
 *
 * @param {string} w
 */
const normalizeWord = w =>
  w
    .replace(/[,\.\d\-\_']+/g, '')
    .trim()
    .toLocaleLowerCase()
process.on('message', async msg => {
  let [words, limit] = msg
  limit = limit > 9 ? 9 : limit < 1 ? 1 : limit
  let loaded = await getWords(words, limit)
  if (!loaded.length)
    loaded = [
      {
        word: 'gibberish',
        defs: [
          {
            def: 'Spoken or written words that have no meaning',
            eg: 'I was so nervous, I just started talking gibberish.',
          },
          {
            def: 'Confused or meaningless speech or writing',
            eg:
              'See if you can make out what he’s saying – it sounds like gibberish to me.',
          },
        ].slice(0, limit),
      },
    ]
  // .map(format)
  const ts = JSON.stringify(loaded)
  process.send(ts)
})

function getWords(words, limit) {
  return Promise.all(
    words.map((w = '') => scrape(normalizeWord(w), limit))
  ).then(r => r.filter(x => (!!x ? !!x.word : false)))
}
