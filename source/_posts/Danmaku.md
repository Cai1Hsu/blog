---
title: '[Danmaku\!] 开发记录'
date: 2023-11-05 09:14:46
tags: ['Game Engine', 'SDL', 'OpenGL']
---

A simple game engine to create stg game (and other 2D game).

记录 `DE` 开发过程中的想法和设计思路。

## Repo
[Cai1Hsu/DanmakuEngine](https://github.com/Cai1Hsu/DanmakuEngine)

## InputHander
We use a InputHander to handle all kinds of input, although we only have to handle keyboard currently.
It handles `TopKeyboardHandler` directly. Then, the `TopKeyboardHandler` sends the event to the `UserkeyboardHandler` which belongs to the `screen` at the top of the `ScreenStack`.

## 基于 `Screen` 的逻辑设计
currently finished

### Structure

Root -> ScreenStack

ScreenStacks
{
    loading screen,
    main screen,
    selecting screen,
    game screen
}

We only focus the top screen in the stack, when it was clear, the game quit.

This structure make is harder to add animation when we have to add animations for screen switching, but there is a *hack* solution.

## Update chain
currently finished

## Render
use SDL as backend to simplify development

## Scheduling
Scheduler

### Transformation
#### Transformer
##### Transformation functions
#### Animation
like those in WPF

## Bindables?
support multiple bindings.

## rewrite dependency container

## rewrite argument parser
separate paramaters and arguments


