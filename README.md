# ruff

ruff is a little tool to help my dad quickly find info in a .csv file.

It quickly filters rows based on search text and lets you configure a command
to run for a row.

ruff is cross platform. No Operating System Left Behind.

## How To Enter a Command

When you load a .csv file, the first row is used as the headers. Each header
is available as a variable in the command. So if the first row of your .csv
file looks like this:

```
Name,PC,Number,Address
```

Then you have these variables available to you in your command:

 * `$(Name)`
 * `$(PC)`
 * `$(Number)`
 * `$(Address)`

If you have a folder for each item on your computer and you want the command to
open the folder to it, it might look something like this:

`explorer C:\Some\Folder\$(Number)`
