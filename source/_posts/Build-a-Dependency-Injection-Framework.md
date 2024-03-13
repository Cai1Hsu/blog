---
title: [UNFINISHED] 编写一个利用代码生成器的依赖注入框架
date: 2024-03-13 21:23:30
tags:
---

本文是我在设计框架时编写的，目前文章尚未完成，已有的内容可能不完整，也可能在未来被修改或删除。

## 从依赖注入说起

尽管依赖注入的作用，DIP，IoC这些概念不是本文讨论的重点，它们仅仅是你使用依赖注入的理由，但是要编写一个依赖注入框架，首先需要了解依赖注入的概念。

### 什么是依赖注入
简单地说，依赖注入是一个将依赖对象注入到类中的过程。再具体一点，就是将类的依赖的对象赋值的过程。然而不同的是，这个过程是手动完成的？还是自动完成的？是通过构造函数？还是通过外部直接赋值？还是通过其他方式？显然，为了实现高效的依赖注入，我们需要一个框架来帮助我们完成这些工作。

### 依赖注入框架的工作原理
由于.NET的传统是通过构造函数来实现依赖注入，我们在此以这种方法为例，来说明依赖注入框架的工作原理（尽管我们要实现的框架不使用这种方式）。让我们先来看一段使用依赖注入的代码:

假设我们有这些类：
```Csharp
class HighLevelService(Module module)
{
    private readonly LowLevelModule _modeule = module;

    public void Work()
        => _modeule.DoWork();
}


class Module : LowLevelModule
{
    public void DoWork()
    {
    }
}

interface LowLevelModule
{
    void DoWork();
}
```

在这个样例中，我们通过`HighLevelService`来实现某项功能，但是`HighLevelService`需要某个低级模块`LowLevelModule`来完成这项功能。

让我们来观察一下使用依赖注入框架的代码，这里以`Microsoft.Extensions.DependencyInjection`为例：
```Csharp
using Microsoft.Extensions.DependencyInjection;

ServiceCollection services = [];

services.AddTransient<HighLevelService>();

services.AddTransient<LowLevelModule, Module>();

var provider = services.BuildServiceProvider();

var service = provider.GetRequiredService<HighLevelService>();

service.Work();
```

不管是什么框架，我们的工作流程都是以下几步：
1. 创建一个服务集合，用于记录所有服务的注册信息。
2. 注册服务，将服务的类型和生命周期注册到服务集合中。
3. 构建服务提供者，将服务集合构建成一个服务提供者。
4. 获取服务，通过服务提供者获取服务。
5. 使用服务。

但还没完，我们还有许多问题待解决：
1. Transient, Scoped, Singleton这些生命周期是如何实现的？ 
2. ServiceProvider的内部是如何实现的？
3. 如何在框架中检测一个类的依赖有哪些？
4. 如何创建出这些依赖？

### 依赖反射的依赖注入实现
对于3，4,传统的依赖注入框架通过反射来实现。

对于4，显然我们熟悉的`new`不能够支持我们的需求，我们需要一个更加灵活的方式来创建对象。在C#中，我们可以通过反射来创建对象。
```Csharp
Activator.CreateInstance(typeof(Module), new object[] { /*params of the ctor*/});
```
依赖反射，只要有一个类的类型，以及参数，我们就可以创建出这个类的实例。

对于3: 我们通过反射来获取一个类的构造函数，然后获取这个构造函数的参数列表，这些参数的类型就是这个类的依赖。对于每个依赖，我们都像上文一样递归地获取它的构造函数，直到没有依赖为止。

不过，这种方式有一个问题，就是性能问题。反射是一个非常慢的操作，而且我们每次都要通过反射来获取构造函数，这是一个非常耗时的操作。所以我们需要一个更加高效的方式来实现依赖注入。

.NET提供了源生成器，我们可以通过源生成器来生成代码（包括`Source Generator`和`IIncrementalGenerator`，前者由于性能问题正逐渐被淘汰），这样我们就可以在编译时生成代码，而不是在运行时生成代码。这样我们就可以避免反射的性能问题。

### 依赖注入框架的实现

#### 利用源生成器的依赖注入

但是源生成器不是万能的。由于源生成器只能生成新的代码，而不能修改现有的代码。我们不得不考虑一下如何实现对字段的赋值，或者将依赖传入构造函数。

#### 通过字段赋值的依赖注入

对于使用字段赋值的依赖注入，我们只能在这个类的内部对依赖字段进行赋值。所以我们需要将这个类标记为`partial`，然后为这个类添加方法，利用这个方法对类的字段的访问性来实现依赖注入。

现在让我们回到上文中使用依赖注入的例子：

```Csharp
class HighLevelService
{
    private readonly LowLevelModule _modeule;

    public void Work()
        => _modeule.DoWork();
}
```

要让框架知道哪些是我们要注入的字段，我们可以定义一个`InjectAttribute`，然后在这个字段上添加这个属性。

```Csharp
class HighLevelService
{
    [Inject]
    private readonly LowLevelModule _modeule;
}
```

这样，在源生成器分析代码时，我们就知道了哪些是我们要注入的字段。现在，假设我们已经实现了这个框架的其他所有部分，我们可以通过`Provider.GetRequiered<LowLevelModule>()`来获取依赖。我们要如何把获取的依赖赋值给`_module`呢？前文提到，我们需要依赖`HighLevelService`自身的方法来获取对依赖字段的访问性。所以我们可以用源生成器生成一个这样的方法：

```Csharp
// 我们编写的代码
// HighLevelService.cs
partial class HighLevelService
{
    private readonly LowLevelModule _modeule;

    public void Work()
        => _modeule.DoWork();
}

// 生成的代码
// HighLevelService_DI.g.cs
partial class HighLevelService
{
    public void ActivateDICandidate(ServiceProvider provider)
    {
        _module = provider.GetRequiredService<LowLevelModule>();
    }
}
```

这样在原理上似乎已经可行了，但是我们还有一个问题：我们如何调用这个方法呢？或者说我们应该让谁来调用这个方法呢？显然不能是`HighLevelService`，这样的依赖注入丧失了一部分的自动化。所以这个重任就落到了我们的框架上。我们需要在`ServiceProvider`中调用这个方法。但是框架如何知道这个方法的存在呢？或者说，我们要求所有能够依赖注入的类都有这个方法，那么这样答案就很显然了。我们可以定义一个接口`ICandicate`:
```Csharp
interface ICandicate
{
    void ActivateDICandidate(ServiceProvider provider);
}
```

然后，在使用`Provider.GetService<HighLevelService>()`时，我们的框架便可以调用这个方法。

现在看起来我们已经有了一个相当可行的方案。但我们还有一个问题没有解决：如何创建这个`HighLevelService`对象，以及其他依赖对象。在下文中我们会讨论这个问题。

#### 通过构造函数的依赖注入

对于使用构造函数的依赖注入，我们不得不考虑一个问题：我们如何不依赖反射来创建对象。由于我们不使用反射，因此`new`是我们唯一的选择。那么问题就变成了在哪里使用`new`。

其实解决方案不止一种。在介绍我们的方法前，我打算为大家介绍一个颠覆我们对于依赖注入框架的理解的依赖注入库[Pure.DI](https://github.com/DevTeam/Pure.DI)。事实上，他们自称，这根本就不是一个库或者框架。这个库通过分析依赖路径，创建对象图，不依赖任何容器和框架。也就是说，对于我们上文中的例子：
```Csharp
class HighLevelService(Module module)
{
    private readonly LowLevelModule _modeule = module;

    public void Work()
        => _modeule.DoWork();
}


class Module : LowLevelModule
{
    public void DoWork()
    {
    }
}

interface LowLevelModule
{
    void DoWork();
}
```

这个框架生成一个这样的代码：

```Csharp
partial class Comsumption
{
    // ...

    public IService Root
    {
        get
        {
            return new Service(
                new HighLevelService(
                    new Module()));
        }
    }

    // ...
}
```

核心就在于

```Csharp
return new Service(
    new HighLevelService(
        new Module()));
```

这是通过算法分析依赖路径生成的代码。这样只需要获取`Comsumption.Root`就可以获取到所需的对象，而依赖都通过构造函数注入了。而且，更重要的是，我们真的不需要容器了，要获取我们的对象，我们直接`new Comsumption().Root`，这等价于我们的`Provider.GetRequiredService<HighLevelService>()`。完全地摆脱了容器。

关于这个库的更多信息，请访问[Pure.DI](https://github.com/DevTeam/Pure.DI)。

不过这种方法还是太高级了一些，不适合我们为了了解依赖注入框架工作原理并尝试编写一个使用源生成器的依赖注入框架的目的。所以现在让我们回到我们的问题：如何创建对象。

<!-- TODO -->

#### 容器的实现

现在我们来考虑问题2。与其从问题本身出发，不如从我们的目标出发。假设我们就是`GetService<T>()`方法，对于需要创建的服务，我们直接按照上文的方法创建对象，而对于已经创建的对象，我们需要从某个地方拿到这个对象。而我们是如何获取这个对象的呢？是对象的类型（或者接口）。这之间是一个`Type`到`object`的映射。要具有这样的映射关系，同时还要保证速度。我们很容易想到使用哈希表。事实上，也确实是这样。我们可以使用`Dictionary<Type, object>`来实现这样的映射关系。这样我们就可以在`O(1)`的时间复杂度内获取到我们需要的对象。当然，这只是简单的实现。不过，这就是我们的容器的核心。或者说，这就是一个简单的DI容器。

[这](https://github.com/Cai1Hsu/DanmakuEngine/blob/b67cdfe249a92afb5402d823bac51ddc50a4eefc/DanmakuEngine/Dependency/DependencyContainer.cs)是我在我自己的项目中实现的一个简单的DI容器。事实上就是一个哈希表的包装。也正是因为简单，促使了我计划重新实现一个更加完善的DI框架并在本文中探讨DI框架的设计。

了解了容器的实现原理，我们就可以开始扩展我们的思维，架构整个框架的设计了。

## 依赖注入框架的设计
首先我们需要考虑我们要实现哪些功能。既然我们以经知道了我们的框架的核心是一个简单的DI容器和一些具体的实现细节，那么我们的框架的功能就是对这个容器的操作。但是要实现哪些操作？我们不妨再来看看使用依赖注入框架的代码的工作流程。

### 容器的设计
```Csharp
ServiceCollection services = [];

services.AddTransient<Service1>();
services.AddTransient<Service2>(instanceOfService2);
services.AddTransient<IService, Service3>();

var provider = services.BuildServiceProvider();

provider.GetService<T>();
```

<!-- TODO -->

### 生命周期管理

对于`Transient`，我们只需要在`GetService`时创建对象并返回即可。

对于`Singleton`，如果对象不存在，我们需要创建对象并返回，如果对象存在，我们直接返回对象。

然而`Scoped`就稍显复杂一些了。表面上和`Singleton`一样，如果对象不存在，我们需要创建对象并返回，如果对象存在，我们直接返回对象。但是`Scoped`的对象的生命周期是和`Scoped`相关的。但是`Scoped`对象的`Scope`到底是什么呢？或者说`Scoped`对象的 Lifetime 是什么呢？

<!-- TODO -->

### 注入依赖

<!-- TODO -->