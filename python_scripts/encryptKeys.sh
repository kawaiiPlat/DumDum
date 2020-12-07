#! /bin/zsh
tar -czf - apiKeys.json | openssl enc -e -aes256 -out apiKeys.json.tar.gz
#rm apiKeys.json
