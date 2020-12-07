#! /bin/zsh
openssl enc -d -aes256 -in apiKeys.json.tar.gz | tar xz 
#rm apiKeys.json.tar.gz
