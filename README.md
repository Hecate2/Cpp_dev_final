# Cpp_dev_final
final project of course "Design Method and Develop Environment of Software" 

## Clone the repository
Follow [this link](https://help.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh) to create public-private key on your workstation, and update your github account with the public key.

```shell script
git clone --recurse-submodules git@github.com:Hecate2/Cpp_dev_final.git
git config --global submodule.recurse true
git config --global push.recurseSubmodules on-demand
```

## Suggested development flow
*Correct deployment is as important as elegant codes.*  
0. Clone the on-line repository with command lines.  
1. Always use git! Please avoid downloading zip from github and committing through your browser in cooperated development.  
2. Before you are to start coding:  
```shell script
git pull origin master
```
for **ONLY ONCE**. This command merges the on-line version into your local version, ensuring that your local repository does not fall too much behind the on-line updates.  
And you may choose to write codes in a new branch.  
3. When coding:  
Edit codes directly in your local repository.  
4. Pushing your codes to the on-line repository:  
**Test** your codes carefully to ensure that the online master branch still works after your commitment of codes!  
Use a git GUI tool to **commit** your changes. The tool may also help you check or revert your changes. After committing, **push** your codes by running:  
```shell script
git push origin your_local_branch:remote_branch
```
for example:  
```shell script
git push origin master:new-feature
```
Then, **open a pull request** on `github` from *the new remote branch* (which refers to `new-feature` in the example above), and request a code review from your teammates.  
When approved by teammates, help yourself to merge the pull request on `github`.  

## Suggested encoding and end of line
*Different encoding leads to garbled text in code files; different end of line makes it impossible to check the difference between commits*  
Encoding: `GBK`  
End of line: `\r\n`  

