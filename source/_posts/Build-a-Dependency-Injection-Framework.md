---
title: "[UNFINISHED] 编写一个利用代码生成器的依赖注入框架"
date: 2024-03-13 21:23:30
tags:
---

本文是我在设计框架时编写的，目前文章尚未完成，已有的内容可能不完整，也可能在未来被修改或删除。

## 从依赖注入说起

尽管依赖注入的作用，DIP，IoC 这些概念不是本文讨论的重点，它们仅仅是你使用依赖注入的理由，但是要编写一个依赖注入框架，首先需要了解依赖注入的概念。

### 什么是依赖注入

简单地说，依赖注入是一个将依赖对象注入到类中的过程。再具体一点，就是将类的依赖的对象赋值的过程。然而不同的是，这个过程是手动完成的？还是自动完成的？是通过构造函数？还是通过外部直接赋值？还是通过其他方式？显然，为了实现高效的依赖注入，我们需要一个框架来帮助我们完成这些工作。

### 依赖注入框架的工作原理

由于.NET 的传统是通过构造函数来实现依赖注入，我们在此以这种方法为例，来说明依赖注入框架的工作原理（尽管我们要实现的框架不使用这种方式）。让我们先来看一段使用依赖注入的代码:

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

1. Transient, Scoped, Singleton 这些生命周期是如何实现的？
2. ServiceProvider 的内部是如何实现的？
3. 如何在框架中检测一个类的依赖有哪些？
4. 如何创建出这些依赖？

### 依赖反射的依赖注入实现

对于**问题 4**，显然我们熟悉的`new`不能够支持我们的需求，我们需要一个更加灵活的方式来创建对象。在 C#中，我们可以通过反射来创建对象。

```Csharp
Activator.CreateInstance(typeof(Module), new object[] { /*params of the ctor*/});
```

依赖反射，只要有一个类的类型，以及参数，我们就可以创建出这个类的实例。

对于**问题 3**: 我们通过反射来获取一个类的构造函数，然后获取这个构造函数的参数列表，这些参数的类型就是这个类的依赖。对于每个依赖，我们都像上文一样递归地获取它的构造函数，直到没有依赖为止。

不过，这种方式有一个问题，就是性能问题。反射是一个非常慢的操作，而且我们每次都要通过反射来获取构造函数，这是一个非常耗时的操作。所以我们需要一个更加高效的方式来实现依赖注入。

.NET 提供了源生成器，我们可以通过源生成器来生成代码（包括`Source Generator`和`IIncrementalGenerator`，前者由于性能问题正逐渐被淘汰），这样我们就可以在编译时生成代码，而不是在运行时生成代码。这样我们就可以避免反射的性能问题。

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

现在我需要剧透一下下一节的内容，假设我们的对象已经创建了，我们调用的`GetService<T>()`函数实质上是通过一个`System.Type`的对象映射到`object`对象来获取组件。

那如果对象没有被创建呢？我们可以通过`System.Type`到一个工厂方法的映射来创建对象。

事实上，许多依赖注入框架允许你传入自己的工厂方法用于注册服务，就像下面一样。

```Csharp
services.AddTransient<HighLevelService>(() => new HighLevelService(new Module()));
```

但是为了实现自动化，我们计划让源生成器来生成这个工厂方法。与上文中的`Pure.DI`不同的是，我们不会直接把依赖`new`出来传给`HighLevelService`的构造函数。而是通过我们的框架来`GetRequiredService<Module>()`。

所以事实上我们的源生成器应该生成下面的工厂方法:

```Csharp
Func<Provider, object> _factory = provider =>
{
    var dependency1 = provider.GetRequiredService<Dependency1>();
    var dependency2 = provider.GetRequiredService<Dependency2>();

    return new HighLevelService(dependency1, dependency2);
};
```

这样的话，我们的框架就能够控制依赖的生命周期了。这也是`Pure.DI`目前还没有实现的功能。因为他们将生命周期管理交给了用户，对于`Singleton`类型，你需要自己将对象持久地保存在一个静态字段中。对于`Transient`类型，你可以每次都`new`一个`Comsumption`对象。但是对于`Scoped`类型，他们目前还没有解决方案。当然，对于任何框架来说，`Scoped`类型都是一个比较棘手的问题。

现在，我们需要想办法将每个类型的工厂方法的委托保存起来。显然我们可以用哈希表来储存这些委托。但是我个人认为`RBtree`也是一个不错的选择。
如果我们自定义自己的委托类型，并且重写`GetHashCode`方法（事实上我们的委托可以仅仅通过对象类型`Type`来获取哈希），我们仅仅只需要多花几次计算哈希值的时间，就可以获取到需要的对象。而这几次计算哈希的时间对于构造函数来说是微不足道的。并且你还可以将哈希持久化地储存到字段中(因为我们的哈希仅仅需要和类型 Type 相关，而类型是不会变的)。取而代之的是，我们减少了大量的内存开销。当然，这只是我个人的想法。而且我仅认为你应该在`ServiceAccessor`的映射中使用`RBtree`，而在`System.Type`到`object`的映射中应该使用哈希表。

现在我们知道了如何创建对象，应该用什么数据结构来储存工厂方法。但是还有些实际问题要思考。我们生成的工厂方法应该定义在哪里？假如我们定义在实际要创建的类型的类里面，我们可以为类型生成下面的代码：

```Csharp
partial class HighLevelService
{

    [DIFactory]
    private static Func<Provider, object> _factray = provider =>
    {
        var dependency1 = provider.GetRequiredService<Dependency1>();
        var dependency2 = provider.GetRequiredService<Dependency2>();

        return new HighLevelService(dependency1, dependency2);
    };
}
```

但这仅对于我们自己写的类型有效。假如我们想要将一个`System.Stopwatch`作为一个服务，或者我们的服务依赖`System.Stopwatch`，那怎么办呢？我们不能修改`System.Stopwatch`的源码。因为`partial`仅仅对于`Roslyn`有意义，在`CLR`层面和`IL`层面都不存在`partial`，分部类都被`Roslyn`整合到了一起，因此我们无法生成`System.Stopwatch`的工厂方法。

所以我们生成的工厂方法只能在实际使用框架的项目中，而不能是我们的依赖注入框架。

让我们看看现有的使用源生成器的依赖注入框架是如何解决这个问题的。`osu.Framework`的框架不支持自动类型创建，因此也不使用工厂方法，但是对于有无参构造函数的依赖（可以直接`new()`），会在`Inject`委托中直接构造对象(这也意味着依赖注入框架不能实现生命周期管理)。但是`osu.Framework`的依赖注入框架和框架内的其他代码紧密结合，因此不单独解决这个问题。

让我们再来看看[`pakrym/jab`](https://github.com/pakrym/jab)，在这个项目的`README.md`中，我们可以看到这个库的使用方法：

```Csharp
[ServiceProvider]
[Transient(typeof(IService), typeof(ServiceImplementation))]
internal partial class MyServiceProvider { }
Use the service provider:

MyServiceProvider c = new MyServiceProvider();
IService service = c.GetService<IService>();
```

看到这里，你应该有一个思路了，既然我们的工厂方法只能在实际使用框架的项目中，那我们就在自己的项目中定义一个`partial`类，并标注为`ServiceProvider`。这样源生成器就可以将这个类补充成一个完整的`ServiceProvider`。并且，我们的工厂方法也可以定义在这个类中。为了提高工厂方法的复用性，我们可以用一个静态的容器类来储存工厂方法。然后在Type initializer中将工厂方法添加到容器类中。这样我们不管定义多少个`ServiceProvider`，我们的工厂方法都可以被复用。

这里要说明的一点是，我并没有检查`pakrym/jab`的代码或使用这个库生成的代码，这意味着我并不知道这个库是如何实现的。这只是我根据`README.md`中样例代码的推测。不过，我们确实有了一个切实可行的方案。

如果你希望了解其他的使用源生成器的依赖注入框架，可以自行查看一下仓库：
- [pakrym/jab](https://github.com/pakrym/jab)
- [Splat.DI.SourceGenerator](https://github.com/reactiveui/Splat.DI.SourceGenerator)

#### 容器的实现

现在我们来考虑**问题 2**。与其从问题本身出发，不如从我们的目标出发。假设我们就是`GetService<T>()`方法，对于需要创建的服务，我们直接按照上文的方法创建对象，而对于已经创建的对象，我们需要从某个地方拿到这个对象。而我们是如何获取这个对象的呢？是对象的类型（或者接口）。这之间是一个`Type`到`object`的映射。要具有这样的映射关系，同时还要保证速度。我们很容易想到使用哈希表。事实上，也确实是这样。我们可以使用`Dictionary<Type, object>`来实现这样的映射关系。这样我们就可以在`O(1)`的时间复杂度内获取到我们需要的对象。当然，这只是简单的实现。不过，这就是我们的容器的核心。或者说，这就是一个简单的 DI 容器。

[这](https://github.com/Cai1Hsu/DanmakuEngine/blob/b67cdfe249a92afb5402d823bac51ddc50a4eefc/DanmakuEngine/Dependency/DependencyContainer.cs)是我在我自己的项目中实现的一个简单的 DI 容器。事实上就是一个哈希表的包装。也正是因为简单，促使了我计划重新实现一个更加完善的 DI 框架并在本文中探讨 DI 框架的设计。

了解了容器的实现原理，我们就可以开始扩展我们的思维，架构整个框架的设计了。

## 依赖注入框架的设计

首先我们需要考虑我们要实现哪些功能。既然我们以经知道了我们的框架的核心是一个简单的 DI 容器和一些具体的实现细节，那么我们的框架的功能就是对这个容器的操作。但是要实现哪些操作？我们不妨再来看看使用依赖注入框架的代码的工作流程。

### 生命周期管理

对于`Transient`，我们只需要在`GetService`时创建对象并返回即可。

对于`Singleton`，如果对象不存在，我们需要创建对象并返回，如果对象存在，我们直接返回对象。

然而`Scoped`就稍显复杂一些了。表面上和`Singleton`一样，如果对象不存在，我们需要创建对象并返回，如果对象存在，我们直接返回对象。但是`Scoped`的对象的生命周期是和`Scoped`相关的。但是`Scoped`对象的`Scope`到底是什么呢？或者说`Scoped`对象的 Lifetime 到底是依据谁的生命周期呢？

在`Microsoft.Extensions.DependencyInjection`中，当我们创建了一个`ServiceProvider`，我们会发现这个 provider 有一个`CreateScope()`的方法，这意味着`Scoped`对象的生命周期是和 Provider 相关的。同时，当我们查看`ServiceCollection`的代码，我们会发现`ServiceCollection`只是一个登记了`ServiceDescriptor`的集合，这也暗示着一切的生命周期的管理和实现都是在`ServiceProvider`中的。

那让我们写一些片段代码来研究一下`Scope`到底是什么。

首先我们来定义一个服务对象，这个对象只需要能够区分出不同的实例即可。

```Csharp
class Service
{
    private readonly Guid _id = Guid.NewGuid();

    public override string ToString() => _id.ToString();
}
```

然后测试以下代码：

```Csharp
ServiceCollection services = [];

services.AddScoped<Service>();

var provider = services.BuildServiceProvider();

while (true)
{
    Console.WriteLine(provider.GetService<Service>());
    Thread.Sleep(500);
}
```

运行代码，我们会发现每一次得到的都是同样的输出。

```bash
caiyi@archlinux ~/r/di> dotnet run
41f8f95d-7e1e-4cd3-9040-0f267e3cc1cb
41f8f95d-7e1e-4cd3-9040-0f267e3cc1cb
41f8f95d-7e1e-4cd3-9040-0f267e3cc1cb
^C
```

让我们做一些修改:

```Csharp
ServiceCollection services = [];

services.AddScoped<Service>();

while (true)
{
    var provider = services.BuildServiceProvider();
    Console.WriteLine(provider.GetService<Service>());
    Thread.Sleep(500);
}
```

再次运行代码，我们会发现每一次得到的都是不同的输出。

```bash
caiyi@archlinux ~/r/di> dotnet run
5015136f-c9f1-4e94-a816-a3b1a59e5d8d
0a8c1797-94c1-48ea-94a8-671d237169a6
c6933d77-a09e-4d96-b0f7-5c289ab1761e
1b0794d0-07a2-4eec-a5fb-2bb07e7d7a2d
0e717435-823b-4553-a8ec-e538ca9946ea
^C⏎
```

再试试我们提到的`CreateScope()`方法：

```Csharp
ServiceCollection services = [];

services.AddScoped<Service>();

var rootProvider = services.BuildServiceProvider();

while (true)
{
    var scope = rootProvider.CreateScope();
    
    Console.WriteLine(scope.ServiceProvider.GetService<Service>());
    Thread.Sleep(500);
}
```

运行代码，我们会发现每一次得到的都是不同的输出。

```bash
caiyi@archlinux ~/r/di> dotnet run
585107ee-8e26-475a-a28c-93cbca3209c8
8295df36-4e89-491f-bfc4-45cfacf40bd6
3a3ca78f-7a66-44bb-a6b3-98d71ea59bb1
5d3293d8-8c49-4665-812a-3ded3fe81110
817aadaf-3c95-471a-9a9f-f01a801f15d9
^C
```

现在你应该能理解了，`Scoped`对象的生命周期是和一个`IServiceScope`相关的，而`IServiceScope`是和`ServiceProvider`相关的。所以`Scoped`对象的生命周期是和`ServiceProvider`的生命周期相关的。也就是说，`Scoped`对象的生命周期是和`ServiceProvider`的生命周期一样的。

事实上，`IServiceScope`里有一个`ServiceProvider`，默认情况下，我们使用`BuildServiceProvider`创建的`ServiceProvider`只是省略了包含它的`IServiceScope`。而`ServiceProvider`的`CreateScope`方法依然是通过创建它的`ServiceCollection`的`CreateScope`方法。如果你直接查看`CreateScope`方法的源码，你会发现只有一行代码：

```Csharp
public static IServiceScope CreateScope(this IServiceProvider provider)
{
    return provider.GetRequiredService<IServiceScopeFactory>().CreateScope();
}
```

上面的样例代码中`rootProvider`的命名可能会让你误解，但是`rootProvider`并不是`ServiceProvider`的根，而是`ServiceProvider`的一个实例。事实上，通过`CreateScope`方法创建的`IServiceScope`的`ServiceProvider`的对象们之间是平行关系，和`rootProvider`之间也是平行关系。

考虑到`IServiceScope`和`ServiceProvider`极为紧密的关系，我们可以理解为`Scoped`对象的生命周期是和`ServiceProvider`的生命周期一样的。

但现在又引出了一个问题，既然Singleton服务在任何时候都返回同一个实例，那么不同的`ServiceProvider`也会返回同一个实例。这意味着Singleton不是储存在`ServiceProvider`里的。那么不同的`ServiceProvider`是如何返回同一个实例的呢？

查看`ServiceProvider`的源码，我们可以发现`ServiceProvider`内部有一个这样的字段：

```Csharp
internal ServiceProviderEngineScope Root { get; }
```

那么关于生命周期管理的一切疑问都已经解决了。

### 注入依赖

对于注入依赖，我打算支持构造函数注入和字段注入。

#### 构造函数注入
对于构造函数注入，前文我们已经讨论过了，我们使用源生成器，扫描Candicate的构造函数的参数列表，在生成的工厂方法中，将每个参数都通过传入的provider获取到依赖，然后构造出new语句并返回。它应该是类似下面的形式：
```Csharp
// 对于其他服务的工厂方法...

    _serviceFactories.Add(typeof(Service1), (provider) =>
    {
        var dependency1 = provider.GetRequired<Dependency1>();
        var dependency2 = provider.GetRequired<Dependency2>();
        var dependency3 = provider.GetRequired<Dependency3>();

        return new Service1(dependency1, dependency2, dependency3);
    });

// 对于其他服务的工厂方法...
```

递归地，最终一定会找到具有无参构造函数的依赖或者已经传入了工厂方法的依赖。这样我们就能构造并注入所有的依赖了。

#### 字段注入

在C#中，我们不需要单独的setter，因为你可以定义成属性，而属性和字段都可以直接赋值。因此我们事实上支持所有的注入方式。

前文提到，为了支持私有字段和私有属性的注入，我们需要在Candicate的内部生成方法，利用内部方法对私有字段的可访问性提供对字段的赋值能力。现在，考虑下面的一个类：

```Csharp
partial class HighLevelService
{
    [Inject]
    private Dependency1 _dependency1 = null!;

    [Inject]
    private Dependency2 _dependency2 { get; set; } = null!;
}
```

我们可以生成下面的代码：

```Csharp
partial class HighLevelService : IDICandicate
{
    virtual void ActivateCandicate(ServiceProvider provider)
    {
        this._dependency1 = provider.GetRequired<Dependency1>();
        this._dependency2 = provider.GetRequired<Dependency2>();
    }
}
```

注意我这里将`ActivateCandicate`方法标记成了`virtual`，因为我们不止希望注入基类，还希望能够注入子类。这样的话，子类生成的`ActivateCandicate`只需要调用一下`base.ActivateCandicate()`就可以将基类的字段注入依赖。

然后，当我们通过`ServiceProvider`调用`GetRequired<HighLevelService>`时，`ServiceProvider`先通过构造函数注入的方式，使用工厂模式构造实例，然后在将实例强转为`IDICandicate`并调用`ActivateCandicate`。

### 容器API的设计

现在我们对一个依赖注入框架的实现的每个部分都非常清楚了，现在我们可以开始设计我们的 API 提案了。

这里以`Microsoft.Extensions.DependencyInjection`的语法为例，先考虑我们使用DI的基本工作流：

```Csharp
ServiceCollection services = [];

services.AddTransient<Service1>();
services.AddTransient<Service2>(instanceOfService2);
services.AddTransient<IService, Service3>();

var provider = services.BuildServiceProvider();

provider.GetService<T>();
```

1. 我们需要一个Services登记表，可以添加，删除Service记录信息。现在我们假设这就是一个`List<ServiceDescriptior>`

2. 这个登记表可以构造出我们的ServiceProvider. 基本上，对于我们这种使用源生成器的实现来说，我们只需要将`List<ServiceDescriptior>`拿过来就能够完成构造。当然我们会出于性能原因把这个`List`转换成`Dictionary`。

3. `ServiceProvider`本质上也就一个功能：获取对象。我们直接用伪代码来表示吧：

```Csharp
class ServiceProvider
{
    // 用于管理单例服务
    private readonly RootEngine _root;
    
    // 事实上，由于我们的Candicate都实现IDICandicate，我们可以直接储存IDICandicate。
    private readonly Dictionary<Type, object> _scopeds;

    // 前文提到我们也可以使用RBTree
    private readonly Dictionary<Type, FactoryDelegate> _factories;

    // 储存服务的生命周期等信息
    private readonly Dictionary<Type, ServiceDescriptor> _services;

    public object GetService(Type type)
    {
        if (!_services.ContainsKey(type))
            throw new Exception("服务未注册");

        var service = _services[type].Lifetime switch
        {
            Lifetime.Singleton => _root.GetSingleton(type),
            Lifetime.Scoped => GetScoped(type),
            Lifetime.Transient => GetTransient(type)
        };

        ((IDICandicate)service).ActivateCandicate(this);

        // do some hooks here

        return service;
    }

    // 对于RootEngine的GetSingleton也类似，只是储存实例的容器的生命周期不同导致了服务实例的生命周期不同
    private object GetScoped(Type type)
    {
        if (_scopeds.ContainsKey(type))
            return _scopeds[type];

        var service = _factories[type].Invoke(this);

        _scopeds.Add(type, service);

        return service;
    }

    private object GetTransient(Type type)
        => _factories[type].Invoke(this);
}
```

这样就实现了整个依赖注入框架和容器。当然，我们还需要编写源生成器和Analyzer来提供其他行为。不过由于本文只是从理论层面解决编写利用源生成器的依赖注入框架所面临的问题，实际的实现在此不会讨论。并且使用源生成器对类进行分析的过程和是否反射对类分析的过程相当相似。