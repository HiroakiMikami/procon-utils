const spawn = require('child_process').spawn

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

module.exports.exec = exec

module.exports.execWithoutOutput = function (command, args) {
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

module.exports.mkdir = function(dir) {
    return exec('mkdir', ['-p', dir])
}