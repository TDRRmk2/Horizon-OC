git update-ref -d refs/remotes/origin/HEAD
git remote prune origin
git fetch origin
git symbolic-ref refs/remotes/origin/HEAD refs/remotes/origin/main
git remote show origin