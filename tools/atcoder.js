const fs = require('fs')
const path = require('path')
const cheerio = require('cheerio')

const { exec, execWithoutOutput } = require('./common')

module.exports = {}
module.exports.exec = exec
module.exports.getContestFiles = (root, name) => {
    let problems = []
    if (name.match(/abc\d+/)) {
        problems = ['a', 'b', 'c', 'd']
    } else if (name.match(/arc\d+/)) {
        problems = ['c', 'd', 'e', 'f']
    } else if (name.match(/agc\d+/)) {
        problems = ['a', 'b', 'c', 'd', 'e', 'f']
    } else {
        problems = ['a', 'b', 'c', 'd', 'e', 'f']
    }

    return problems.map(problem => {
        return path.resolve(root, 'src', 'atcoder', name, `${problem}.cc`)
    })
}
module.exports.downloadTestCases = (root, name) => {
    return exec('python3', [path.resolve(path.dirname(process.argv[1]), 'allproblems.py'), `http://${name}.contest.atcoder.jp/assignments`])
    .then(data => {
        const $ = cheerio.load(data)
        let results = []
        $('#outer-inner table tbody tr td.center a').each(function (idx, elem) {
            const problemName = $(this).text().toLowerCase()
            const link = $(this).attr('href')
            
            const result = execWithoutOutput('oj', [
                'dl',
                '-f', path.resolve(root, 'src', 'atcoder', name, `test-${problemName}`, 'sample%i.%e'),
                `http://${name}.contest.atcoder.jp${link}`
            ])
            results.push(result)
        })
        return Promise.all(results)
    })
}
