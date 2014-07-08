# ruff

ruff is a little tool to help my dad quickly find info in a .csv file.

It quickly filters rows based on search text and lets you configure a command
to run for a row.

ruff is cross platform. No Operating System Left Behind.

![](http://superjoe.s3.amazonaws.com/temp/ruff-1.png)
![](http://superjoe.s3.amazonaws.com/temp/ruff-2.png)

## How To Enter a Command

### Format

The basic format looks like this:

```
program arg1 "arg with a space" arg3
```

The first thing is the path to the executable. After that is space-separated
arguments. If you want an argument to have a space, you may use single quotes
(`'`) or double quotes (`"`). If you need a literal single quote in a single-
quoted string, use two in a row, `'like ''this'''`. Same thing for double
quotes.

Consider running your command in a shell for more features. For example, on
Linux you might do `bash -c "command arg arg arg"` and on Windows you might do
`cmd.exe /c "command arg arg arg"`.

### Column Parameters

When you load a .csv file, the first row is used as the headers. Each header
is available as a variable in the command. So if the first row of your .csv
file looks like this:

```
Name,PC,Number,Address
```

Then you have these variables available to you in your command:

 * `${Name}`
 * `${PC}`
 * `${Number}`
 * `${Address}`

If you have a folder for each item on your computer and you want the command to
open the folder to it, it might look something like this:

`explorer C:\Some\Folder\${Number}`

If you need to enter a literal `$`, use 2 of them like this: `$$`.

Inside the `${}`, `$` are already literal. Use `}}` if you need a literal `}`.
