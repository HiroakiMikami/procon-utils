import * as fs from "fs"
import * as AtCoder from "atcoder.js"
import * as path from "path"
import { promisify } from "util"
import * as cheerio from "cheerio"

async function main() {
    const sessionfile = process.argv[2]
    const cachedir = process.argv[3]
    const command = process.argv[4]
    const domain = process.env["ATCODER_DOMAIN"] || "https://atcoder.jp"

    let session: AtCoder.Session = new AtCoder.Session()
    if (await promisify(fs.exists)(sessionfile)) {
        const data = await promisify(fs.readFile)(sessionfile, "utf8")
        session = new AtCoder.Session(data)
    }
    const atcoder = new AtCoder.AtCoder(session, { cache: { cachedir, maxMemoryEntries: 0 }, url: { atcoder: domain }})

    if (command == "login") {
        const username = process.argv[5]
        const password = process.argv[6]
        await atcoder.login(username, password)
    } else {
        const contestName = process.argv[5]
        const taskName = process.argv[6]

        const contest = atcoder.contest(contestName)
        const tasks = await contest.tasks()
        const task = tasks.find(task => task.name[0].toLowerCase() == taskName)

        if (command == "task_url") {
            console.log(`${domain}/${contestName}/tasks/${task.id}`)
        } else if (command == "score") {
            const score = await contest.task(task.id).score()
            console.log(score)
        } else if (command == "testcases") {
            const targetDir = process.argv[7]
            await promisify(fs.mkdir)(targetDir)
            const examples = await contest.task(task.id).examples()
            let i = 0
            for (const example of examples) {
                const input = cheerio.load(example.input)("pre").text()
                const output = cheerio.load(example.output)("pre").text()
                await promisify(fs.writeFile)(path.join(targetDir, `sample${i}.in`), input)
                await promisify(fs.writeFile)(path.join(targetDir, `sample${i}.out`), output)

                i += 1
            }
            console.log(i)
        } else if (command == "submit_results") {
            console.error("Not implemented yet")
        }
    }

    await promisify(fs.writeFile)(sessionfile, JSON.stringify(session.toJSON()))
}

main().catch(console.error)
