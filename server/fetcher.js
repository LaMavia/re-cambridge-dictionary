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
  const [words, limit] = msg
  const loaded = (await Promise.all(
    words.map((w = '') => scrape(normalizeWord(w), limit))
  )).filter(x => (!!x ? !!x.word : false))
  // .map(format)
  const ts = JSON.stringify(loaded)
  process.send(ts)
})
