// @ts-check
const { Flow, send, initLogger } = require('river-flow')
const cp = require('child_process')
const u = require('url')
const qs = require('querystring')
const path = require('path')
const fs = require('fs')

const m = new Flow()
m.public = path.resolve(
  __dirname,
  /server/.test(__dirname) ? '../client/' : 'client/'
)

m.gate(({ req, res }) => {
  res.setHeader('Access-Control-Allow-Origin', '*')
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With')
})
const logger = initLogger('Logger', 'italic')
m.gate(({ req }) => {
  logger(`[${req.method}]> ${req.url} ${req.statusCode}`)
})

m.post(
  'api',
  async ({ req, res, supervisor }) =>
    new Promise((fuf, rej) => {
      let { query } = u.parse(req.url)
      const limit = +qs.parse(query)['limit']
      const ws = JSON.parse(Object.keys(req.body)[0]).words.filter(Boolean)
      if(ws.length === 0) ws.push("emptiness")
      console.dir(ws, { colors: true, depth: 4 })
      const fetcher = cp.fork('server/fetcher.js')
      fetcher.on('message', ws => {
        send(res, ws)
        fuf({
          continue: false,
        })
        fetcher.kill()
        // console.dir(ws)
      })
      fetcher.send([ws, limit])
    })
)
let indexc
m.get(
  '/',
  ({ req, res, supervisor: app }) =>
    new Promise((fuf, rej) => {
      if (indexc) res.write(indexc)
      else {
        try {
          fs.readFile(
            `${app.public}/index.html`,
            { encoding: 'utf-8' },
            (err, d) => {
              if (err) throw new Error(`405 Server Error: ${err}`)
              indexc = d
              res.write(indexc)
              fuf()
            }
          )
        } catch (err) {
          res.write(err)
          fuf()
        }
      }
    })
)

// @ts-ignore
m.listen(process.env.PORT || 8080)
