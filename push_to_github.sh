#!/usr/bin/env bash
set -euo pipefail

REPO_URL="https://github.com/anurag03-10/IIT-ISM-ADSA-Assignments.git"
BRANCH="main"

echo "Preparing to push workspace to ${REPO_URL} on branch ${BRANCH}"

if [ ! -d .git ]; then
  echo "Initializing git repository..."
  git init
fi

git add -A

if git rev-parse --verify HEAD >/dev/null 2>&1; then
  git commit -m "Update ADSA assignment solutions (26ET0006)" || echo "No changes to commit"
else
  git commit -m "Add ADSA assignment solutions (26ET0006)"
fi

# Ensure branch name
git branch -M $BRANCH || true

if git remote get-url origin >/dev/null 2>&1; then
  echo "Remote 'origin' exists. Setting URL to ${REPO_URL}"
  git remote set-url origin "$REPO_URL"
else
  git remote add origin "$REPO_URL"
fi

echo "Fetching remote and attempting to rebase..."
git fetch origin $BRANCH || true
git pull --rebase origin $BRANCH || true

echo "Now pushing to origin/$BRANCH. Provide credentials (username + PAT) if prompted." 
git push -u origin $BRANCH

echo "Push complete. If this failed due to authentication, consider using an SSH remote or a Personal Access Token (PAT)."
