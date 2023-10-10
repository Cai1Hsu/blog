#!/bin/bash

# 配置环境
if ! command -v hexo &> /dev/null
then
  echo '[!] Installing hexo using npm'
  npm install -g hexo-cli
  npm install
fi

echo '[!] hexo installed, starting build'
# 使用新的渲染器
# npm un hexo-renderer-marked --save
# npm i hexo-renderer-markdown-it --save
# 处理图片
# npm i hexo-image-link --save

# 复制文章到指定目录
# cd ../
# cp -r `ls | grep -v hexo | grep -v config.yml | xargs` hexo/source/_posts
# 复制配置文件
# cp config.yml hexo

# 开始构建

hexo generate
