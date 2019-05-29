type q = {
  ws: array(string),
  limit: int
};
let q = [%bs.raw {|
  (() => {
    let q
    if(/\?(.+)/.test(location.href)) {
      q = /\?(.+)/.exec(location.href)[1].split("&")
      .reduce((acc, x) => {
        const [k, v] = x.split("=")
        acc[k] = v
        return acc
      }, {})
    }

    return {
      ws: q && q['words'] ? q['words'] : [],
      limit: q && q['limit'] ? q['limit'] : 1
    }

  })()
|}];
ReactDOMRe.renderToElementWithId(<App/>, "root");