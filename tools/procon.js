const fs = require('fs')
const path = require('path')
const { exec, execWithoutOutput, mkdir } = require('./common')

const base = path.resolve(path.dirname(process.argv[1]), '..')

// Parse command line options
if (process.argv.length < 3) {
    console.error(`${process.argv[0]} [-C <root-dir>] <subcommand> <args>`)
    process.exit(1)
    return ;
}

let root = null
let subcommand = null
let args = []
if (process.argv[2] === '-C') {
    root = fs.realpathSync(process.argv[3])
    subcommand = process.argv[4]
    for (let i = 5; i < process.argv.length; i++) {
        args.push(process.argv[i]);
    }
} else {
    root = fs.realpathSync('./')
    subcommand = process.argv[2]
    for (let i = 3; i < process.argv.length; i++) {
        args.push(process.argv[i]);
    }
}

console.error(`Root Dir: ${root}`)
console.error(`SubCommand: ${subcommand}`)
console.error(`Arguments: ${args.join(', ')}`)

function getFunctions(contestName) {
    return require(`./${contestName}`)
}

switch (subcommand) {
    case 'prepare':
        (function () {
            if (args.length < 2) {
                console.error(`${process.argv[0]} prepare <contest-type(e.g., atcoder)> <contest-name(e.g., arc001)>`)
                process.exit(1)
            }

            const contestType = args[0]
            const contestName = args[1]

            const createTemplate = (filePath) => {
                const common = fs.readFileSync(`${base}/languages/cc/templates/src/common.cc`, 'utf8')
                const main = fs.readFileSync(`${base}/languages/cc/templates/src/main.cc`, 'utf8')

                return mkdir(path.dirname(filePath)).then(() => {
                    return fs.writeFileSync(filePath, common + "\n" + main, 'utf8')
                })
            }
            
            const functions = getFunctions(contestType)
            Promise.all(functions.getContestFiles(root, contestName).map(createTemplate)).then(() => {
                // Update cmake
                process.chdir(path.resolve(root, 'build'))
                return exec('cmake', ['..'])
            })
        })()
        break;
    case 'start':
        (function () {
            if (args.length < 2) {
                console.error(`${process.argv[0]} start <contest-type(e.g., atcoder)> <contest-name(e.g., arc001)>`)
                process.exit(1)
            }

            const contestType = args[0]
            const contestName = args[1]

            const functions = getFunctions(contestType)
            functions.downloadTestCases(root, contestName)
        })()
        break;
    case 'test':
        (function() {
            if (args.length < 1) {
                console.error(`${process.argv[0]} start <target-src>`)
                process.exit(1)
            }
            const target = args[0]
            const dir = path.relative(root, path.dirname(target)).replace(/\.\//, '')
            const ext = path.extname(target)
            const name = path.basename(target, ext)

            const makeTarget = `${dir.split('/').join('-')}-${name}`

            execWithoutOutput('make', [
                '-C', path.resolve(root, 'build'), `${makeTarget}`
            ]).then(() => {
                return execWithoutOutput('oj', [
                    'test',
                    '-c', path.resolve(root, 'build', 'src', makeTarget),
                    '-f', path.resolve(root, dir, `test-${name}`, '%s.%e')
                ])
            })
        })()

        break ;
    default:
        console.error(`Unknown subcommand: ${subcommand}`)
        process.exit(1)
}
