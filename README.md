# OpenGL
## Little Github Instructions

#### Configuring git to your local machine

After registration in GitHub you must onfigure git locally to your computer.
Step one. Set username
```
git config --global user.name "Dachi Darchiashvili"
```
Step two. Set email
```
git config --global user.email darchiashvilidachi@yahoo.com
```
Step tree: Set default upstream branch (I'll explain this later)
```
git config --global push.default current
```
#### Chapter one cloning this repository to your computer.

Clonning is very simple in GitHub.
Step one. Take repository link as shown bellow
![img](http://i.imgur.com/WoogC5Y.png)

Step two. Now it's time to clone
```
git clone <URL HERE>
```
Now you have all the files

For downloading chages use:
```
git pull
```


FOR REPOSITORY DEVELOPERS

 - The best way to wotk in github is branches system. After clonning you are in master branch, but it's not recommended to work directly in main branch. The better way to achieve this is to make new branch `git checkout -b <new branch name>` and you will create new branch.
 - Using `git checkout <branch name>` you can switch branches. For example `git checkout master` will return us in master branch.
 - Every changes stays in branch. For example if I have two branches master and slave, and I'll add some files to slave branch and after that if I'll change branch to master, master would be wothout any changes made in the slave branch.
 
How does system works for adding file.txt in Documents directory (now we are on master branch, to see current brach execute `git branch`, Documents and file.txt both are new files):
 - `git branch` (make sure that we are on master)
 - `git pull` (download newest version of repository(
 - `git checkout -b addFile` (creates new branch addFile)
 - `git branch` (make sure we are on addFile branh)
 - `mkdir Documents`
 - `mv ~/Documents/file.txt Documents/`
 - `git add -A` or `git add Documents` to add files to staging area (in seocnd case the directory must not be ?empty)
 - `git commit -m "uploading file.txt"` (commiting changes, now all changes are locally in addFile branch)
 - `git pull` (uploading to GitHub, if we didn't set push.defaul then system will return warning and corresponding fix command)
 - Now we will go back to GitHub (in browser) and make new pull request for this branch and merge it to master. After that if we will check master branch in GitHub it will be updated with this last changes. To make aviable changes locally we must execute `git pull`
 

Thats all for now :)
