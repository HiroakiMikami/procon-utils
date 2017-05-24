const fs = require('fs')
const path = require('path')
const spawn = require('child_process').spawn

const base = path.resolve(path.dirname(process.argv[1]), '..')

function exec (command, args) {
    return new Promise((resolve, reject) => {
        const cmd = spawn(command, args)

        let stdout = ''
        cmd.stdout.setEncoding('utf8')
        cmd.stdout.on('data', line => {
            stdout += line
        })

        let stderr = ''
        cmd.stderr.setEncoding('utf8')
        cmd.stderr.on('data', line => {
            stderr += line
        })

        cmd.on('close', exitCode => {
            if (exitCode === 0) {
                resolve(stdout)
            } else {
                reject(stderr)
            }
        })
    })
}

function execWithOutput (command, args) {
    return new Promise((resolve, reject) => {
        const cmd = spawn(command, args, { stdio: [process.stdin, process.stddout, process.stderr] })

        cmd.on('close', exitCode => {
            if (exitCode === 0) {
                resolve()
            } else {
                reject()
            }
        })
    })
}


function generateMock (contestName, problems) {
    return new Promise((resolve, reject) => {
        // Make a directory
        fs.mkdir(path.resolve('contest', contestName), err => {
            if (err) {
                reject(err)
            } else {
                resolve()
            }
        })
    }).then(() => {
        return new Promise((resolve, reject) => {
            fs.readFile(path.resolve(base, 'template', 'template.cc'), 'utf8', (err, data) => {
                if (err) {
                    reject(err)
                } else {
                    resolve(data)
                }
            })
        })
    }).then(template => {
        // Generate mock code
        const code = Array.from(problems).map(problem => {
            return new Promise((resolve, reject) => {
                let text = template
                text += 'int body(int &argc, char **argv) {\n\n    return 0;\n}\n'
                fs.writeFile(path.resolve(base, 'contest', contestName, `${problem}.cc`), text, err => {
                    if (err) {
                        reject(err)
                    } else {
                        resolve()
                    }
                })
            })
        })

        return Promise.all(code)
    }).catch(reason => {
        console.log(reason)
    })
}

function downloadTestCases (contestName) {
    return new Promise((resolve, reject) => {
        exec('find', [path.resolve(base, 'contest', contestName), '-name', '*.cc']).then(data => {
            const files = Array.from(data.split('\n'))
            const results = files.map(file => {
                const ext = path.extname(file)
                const problem = path.basename(file, ext)
                return execWithOutput('oj', [
                    'dl',
                    '-f', path.resolve(base, 'contest', contestName, `test-${problem}`, 'sample%i.%e'),
                    `http://${contestName}.contest.atcoder.jp/tasks/${contestName}_${problem}`
                ])
            })

            return Promise.all(results)
        })
    })
}

function executeTestCase (contestName, problem) {
    execWithOutput('oj', [
        'test',
        '-c', path.resolve(base, 'build', 'contest', `${contestName}-${problem}`),
        `-f`, path.resolve(base, 'contest', contestName, `test-${problem}`, '%s.%e')
    ])
}

if (process.argv.length < 3) {
    process.exit(0)
    return ;
}

const subcommand = process.argv[2]

switch (subcommand) {
    case 'prepare':
        (() => {
            if (process.argv.length < 4) {
                process.exit(0)
            }
            const contestName = process.argv[3]
            let args = []
            for (let i = 4; i < process.argv.length; i++) {
                args.push(process.argv[i])
            }

            if (args.length === 0) {
                if (contestName.match(/abc\d+/)) {
                    args = ['a', 'b', 'c', 'd']
                } else if (contestName.match(/arc\d+/)) {
                    args = ['c', 'd', 'e', 'f']
                } else if (contestName.match(/agc\d+/)) {
                    args = ['a', 'b', 'c', 'd', 'e', 'f']
                }
            }

            // Prepare a contest
            generateMock(contestName, args).then(() => {
                // Update cmake
                process.chdir(path.resolve(path.dirname(process.argv[1]), '..', 'build'))
                return exec('cmake', ['..'])
            })
        })()
        break ;
    case 'start':
        (() => {
            if (process.argv.length < 4) {
                process.exit(0)
            }

            // Download test cases from AtCoder
            const contestName = process.argv[3]
            downloadTestCases(contestName)
        })()
        break ;
    case 'test':
        (() => {
            if (process.argv.length < 5) {
                process.exit(0)
            }
            const contestName = process.argv[3]
            const problemName = process.argv[4]
            
            // Run test cases
            executeTestCase(contestName, problemName)
        })()
        break ;
    case 'add-test':
        (() => {
            if (process.argv.length < 5) {
                process.exit(0)
            }
            const contestName = process.argv[3]
            const problemName = process.argv[4]

            console.error('Not Implemented')
        })()
        break ;
    default:
        console.error('invalid subcommand')
}
