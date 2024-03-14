---
title: "IIncrementalGenerator 增量生成器初见"
date: 2024-03-14 21:50:07
tags:
---

上一篇文章中我们详细地讲解了使用源生成器的依赖注入框架的工作原理以及具体实现方法。本文将以这个依赖注入框架为例，介绍如何使用增量生成器。

## 什么是增量生成器？

增量生成器是一种特殊的源生成器，它可以在构建过程中只处理发生变化的文件，而不是所有的文件。这样可以大大提高构建的效率。具体地说，在增量生成器中，我们需要定义一系列的管线来处理文件的变化（通过Provider）。当文件发生变化时，增量生成器会根据这些管线来处理文件的变化。从第二个管线开始，当且仅当前一个管线的处理结果发生变化时，才会执行当前管线。这意味着，如果一个文件的处理结果没有发生变化，那么它后面的管线都不会执行。最后生成代码的部分也不会执行。

据Microsoft所说，生成代码的部分是最耗时的部分。因此，增量生成器可以大大提高构建的效率。

与Source Generator不同的是，Incremental Generator仅仅由一个Initialize方法，在这个方法中，我们要注册处理管线，以及最后的生成代码的部分（类似于Source Generator的Execute方法）。

Source Generator的Execute方法会在每次源代码发生变化时执行，也就是你每次敲击键盘时都会执行。在Incremental Generator中，你注册的管线也同样地会在源代码发生变化时执行，但是不同的是，这些管线是分层的，并且有缓存机制。这意味着，如果一个文件的处理结果没有发生变化，那么它后面的管线都不会执行。最后生成代码的部分也不会执行。

## 如何使用增量生成器？

### 创建源生成器项目

与创建Source Generator类似，首先我们需要创建一个单独的项目。本文中我们为`DanmakuEngine.DependencyInjection`创建一个增量生成器。但是由于我们的生成器不只是为了`DependencyInjection`，我们的项目还有其他地方需要使用增量生成器，因此我们将其命名为`DanmakuEngine.SourceGeneration`。事实上你应该命名为`Analyzers`因为Source Generator和Incremental Generator和Diagnostic Analyzer都是`Analyzer`。我这里是出于避免修改其他地方的命名空间而继续使用`SourceGeneration`。

通过下面的命令，快速创建一个增量生成器项目：
```shell
cd Danmaku!
mkdir DanmakuEngine.SourceGeneration
cd DanmakuEngine.SourceGeneration

dotnet new classlib
rm class1.cs

dotnet add package Microsoft.CodeAnalysis.Analyzers
dotnet add package Microsoft.CodeAnalysis.CSharp
```

然后我们需要在项目文件中进行一些配置：
```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>netstandard2.0</TargetFramework>
    <LangVersion>latest</LangVersion>
    <IsRoslynComponent>true</IsRoslynComponent>
    <EnforceExtendedAnalyzerRules>true</EnforceExtendedAnalyzerRules>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>

  <ItemGroup>
    <PackageReference Include="Microsoft.CodeAnalysis.Analyzers" Version="3.3.4">
      <IncludeAssets>runtime; build; native; contentfiles; analyzers; buildtransitive</IncludeAssets>
      <PrivateAssets>all</PrivateAssets>
    </PackageReference>
    <PackageReference Include="Microsoft.CodeAnalysis.CSharp" Version="4.9.2" />
  </ItemGroup>

</Project>
```

首先是`TargetFramework`，必须为`netstandard2.0`。然后添加一个值为true的`IsRoslynComponent`的项。最后是`EnforceExtendedAnalyzerRules`，设置为`true`。这些配置是必须的，否则你的项目将无法被识别为增量生成器。至于语言版本，你可以根据自己的需要进行设置。这里以最新版本为例。

### 编写你的增量生成器

为你的类创建一个新的文件，命名为`DepedencyInjectionGenerator.cs`，加入下面的代码：

```CSharp
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace DanmakuEngine.SourceGeneration.Generators.DenepdencyInjection;

[Generator]
public class DepedencyInjectionGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext initContext)
    {
        // TODO: Register your pipeline here
    }
}
```

前面说了，增量生成器仅仅由一个Initialize方法，在这个方法中，我们要注册处理管线，以及最后的生成代码的部分。这里我们先留空，后面会详细讲解如何注册处理管线。因此我们当然不能直接像Source Generator那样分析Syntax Tree，我们需要注册管线，在管线中通过`Provider`来处理。

通过`initContext`，我们一共可以访问到6个Provider，以及3个方法。

Providers: 
- `SyntaxProvider`：用于处理语法树。
- `CompilationProvider`：用于处理编译信息，例如编译平台，编译配置(Release/Debug)等。
- `AdditionalFilesProvider`：用于处理非代码的文本文件。
- `MetadataReferencesProvider` 获取有关加载的 DLL 的信息。
- `ParseOptionsProvider` 分析 csproj 项目文件信息
- `AnalyzerConfigOptionsProvider`  

大多数情况下，我们只需要使用`SyntaxProvider`。但是如果你的生成器需要处理编译信息，或者需要处理非代码的文本文件，那么你可能需要使用`CompilationProvider`和`AdditionalFilesProvider`。而其他3种Provider则很少用到，关于它们的用法，官方文档也没有解释。[这篇博客（日语）](https://neue.cc/2022/12/16_IncrementalSourceGenerator.html)对它们的作用有一些解释。

Methods:
- `RegisterImplementationSourceOutput()` 与`RegisterSourceOutput`类似
- `RegisterPostInitializationOutput()` 用于在源生成器和分析内部的初始化阶段之后生成Attribute标记，供内部分析使用
- `RegisterSourceOutput()` 注册连接管道并最终实际生成代码的委托

关于RegisterImplementationSourceOutput的信息请查看一下链接

- [https://github.com/dotnet/roslyn/issues/54723](https://github.com/dotnet/roslyn/issues/54723)
- [https://github.com/dotnet/roslyn/discussions/66885](https://github.com/dotnet/roslyn/discussions/66885)
- [https://www.thinktecture.com/net/roslyn-source-generators-introduction/](https://www.thinktecture.com/net/roslyn-source-generators-introduction/)

不过一般来说我们用不到它。

### 生成一些代码

现在回想一下我们上一篇文章中是如何解决创建对象的工厂方法的定义和保存的？简单地说，我们会在框架中编写一个实际的`ServiceProviderBase`的抽象类，它完成了类似`Microsoft.Extensions.DependencyInjection.ServiceProvider`的所有任务。但是由于我们不能直接为`ServiceProviderBase`生成代码，所以我们要求用户定义一个自己的`ServiceProvider`类型，然后我们生成代码，在这个类中声明和储存工厂方法。

我们的框架要求用户使用一个`[ServiceProvider]`标识一个类是他们自定义的`ServiceProvider`类型，并且希望我们的代码生成器为它生成代码。我们要求用户像下面一样定义一个类：

```CSharp
// Danmaku!/DanmakuEngine/DependencyInjection/ServiceProvider.cs

// 假设我们框架的`ServiceProviderBase`也是在`DanmakuEngine.DependencyInjection`中定义的，只不过不在同一程序集。
// 如果你的项目不叫DanmakuEngine，那么你需要显式地引用这个命名空间。
// using DanmakuEngine.DependencyInjection;

namespace DanmakuEngine.DependencyInjection;

[ServiceProvider]
public partial class ServiceProvider
{
    // 实际的业务逻辑在ServiceProviderBase中
}
```

我们计划让我们编写的增量生成器生成下面的代码:

```CSharp
// <auto-generated/>

namespace DanmakuEngine.DependencyInjection
{
    [global::System.Runtime.CompilerServices.CompilerGenerated]
    partial class ServiceProvider : global::DanmakuEngine.DependencyInjection.ServiceProviderBase
    {
        public ServiceProvider(global::DanmakuEngine.DependencyInjection.ServiceCollection services)
            : base(services)
        {
        }
    }
}
```

你或许已经注意到了，生成的代码与我们平时编写的代码似乎有些不同，我们在第一行用注释标记了这个文件是由代码生成器生成的。并且生成的类也用了`[CompilerGenerated]`标记。除此之外，我们对所有类型都使用了全限定名。这样做是为了避免命名冲突。因为我们无法假设用户的代码会如何命名。

同时我提到了我们生成的是代码文件，不过我要补充的一点是，生成的代码并不会直接写入到文件中，也就是说你的硬盘上不会有生成的代码。如果你想保留生成的代码，你需要在你的项目（不是分析器项目）的配置文件中的`<PropertyGroup>`中添加`<EmitCompilerGeneratedFiles>true</EmitCompilerGeneratedFiles>`。这样生成的代码会被写入到临时文件夹中。这个配置项默认是`false`。一般来说，生成的代码在`obj/<Configuration>/<.net version>/generated/`下。虽然你也可以手动指定生成的代码的输出路径。但我们建议，不要这么做。因为通常会导致生成的代码被多次引用，从而导致编译错误。

接下来让我们回到正题，我们要如何生成这段代码？很简单，遍历项目中的类型，找到有attribute标记并且有`[ServiceProvider]`的类型。然后我们就可以生成代码了。当然，我们还需要做一些检查，比如我们要求用户的`ServiceProvider`类型必须是`partial`的，用户的`ServiceProvider`不能够实现除`ServiceProviderBase`以外的其他类型（事实上我们可以像上面一样什么都不写）。

为了解释注册管线的工作原理，我先不使用简单的方法。

我们先来看看我们最终是如何注册生成代码委托的。查看`RegisterSourceOutput`，我们可以看到，这是一个泛型方法，泛型类型有一个参数`TResult`，方法有两个参数，分别为`IncrementalValueProvider<TSource> source`和`Action<SourceProductionContext, TSource> action`。

source是我们连接前面注册的管线的输入，action是我们的生成代码的委托。我们需要将这个委托注册到管线中，以便在源代码发生变化时执行。这个action委托类似于Source Generator的Execute方法。

需要说明的是，这个TResult仅仅代表我们要传给"`execute`"方法的信息的类型。也就是说，你可以传任意对象给这个委托。但也意味着我们在委托中只能通过这个对象来获取信息。当然你也可以通过闭包来捕获一些外界信息。但是不要捕获前面管线中处理的信息，正确的做法是将管线作为source传入`RegisterSourceOutput`。

前面提到，6种Provider可以用于获取不同的信息，因此在一开始我们需要确定好要使用的Provider。在这个例子中，我们需要使用`SyntaxProvider`来获取语法树。

由于Provider的管线仍然会像Source Generator在你每次修改代码时执行，因此我们需要先快速地筛去不需要处理的信息。

下面我将演示我们如何快速地筛去不需要处理的信息。

```csharp
[Generator]
public class CustomServiceProviderHandler : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext initContext)
    {
        var customServiceProviderPipeline = initContext.SyntaxProvider
            .CreateSyntaxProvider(
                static (syntaxNode, _) => IsClassWithAttribute(syntaxNode),
                static (ctx, _) => FilterCustomServiceProviderType(ctx)
            ).Where(static cds => cds is not null);

        // TODO: pass the pipeline to RegisterSourceOutput 
    }

    static bool IsClassWithAttribute(SyntaxNode syntaxNode)
        => syntaxNode is ClassDeclarationSyntax cds && cds.AttributeLists.Count > 0;

    static ClassDeclarationSyntax FilterCustomServiceProviderType(GeneratorSyntaxContext context)
    {
        var cds = (ClassDeclarationSyntax)context.Node;

        var semanticModel = context.SemanticModel;

        var isServiceProvider = cds.AttributeLists
            .Any(al => .Attributes.Any(
                attr => 
                {
                    var symbol = semanticModel.GetDeclaredSymbol(attr);

                    return symbol?.ToDisplayString(SymbolDisplayFormat.FullyQualifiedFormat) 
                        == "DanmakuEngine.DependencyInjection.ServiceProviderAttribute";
                }));

        return isServiceProvider ? cds : null!;
    }
}
```
可以看到，我们使用`CreateSyntaxProvider`创建了一个`SyntaxProvider`管线。这个方法接受两个委托作为参数，其中第一个委托用于过滤不需要处理的信息(Filter)，第二个委托用于处理信息(Transform)。更准确地说第二个参数是将一个信息转化为其他类型的信息（当然你也可以不转化）。但是如果你仔细看我的代码，你会发现，我在第二个委托中所做的实际是过滤信息。而且最后还又调用了`Where`方法删去第二个委托过滤的信息。这是因为要获取attribute的完整类名，我们需要使用`SemanticModel`，而`SemanticModel`是在`SyntaxProvider`的`Transform`委托中才能获取的。

需要说明的是，这里的`Where`以及`Select`都不是Linq中的方法，而是`IncrementalValueProvider<TSource>`的扩展方法。Linq方法接受集合类型，而我们的管线是`IncrementalValueProvider<TSource>`类型显然不是集合类型。`Where`，`Select`以及`SelectMany`方法实质上是再一次创建了一个管线，连接在`CreateSyntaxProvider`的管线之后。对上一个管线的结果进行一次过滤。

现在我们再来看看这个管线的类型是`IncrementalValueProvider<ClassDeclarationSyntax>`。`IncrementalValueProvider<T>`代表着增量管线对象。这个类型T由`CreateSyntaxProvider`的第二个委托的返回值确定。它代表的是这个管线输出对象的类型。如果我们将这个管线直接传给`RegisterSourceOutput`，那么这个类型T也是我们在`RegisterSourceOutput`中的TResult。当然我们也可以继续连接管线，将这个管线的输出作为下一个管线的输入。这样我们就可以在管线中处理更多的信息。

还要说明的一点是，如果你查看`initContext.CompilationProvider`等其他Provider的方法，你会发现它们都没有类似的`CreateProvider`方法，因为它们的全部信息都是可以通过`initContext.*Provider`直接获取的，也就是说，它们本身就是管线。因此你直接对它们使用`Where`等方法即可过滤出你的信息。

最后，在我们将管线链接到`RegisterSourceOutput`之前，我要回来说说过滤出`ServiceProvider`类型的简单方法。由于根据attribute来过滤语法节点是一个非常常用的操作，而我们却用了这么多代码来实现。因此官方提供了一个更简单的方法`ForAttributeWithMetadataName`。我们通过以下代码即可实现上述所有操作：

```CSharp
[Generator]
public class CustomServiceProviderHandler : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext initContext)
    {
        var customServiceProviderProvider = initContext.SyntaxProvider
            .ForAttributeWithMetadataName(
                "DanmakuEngine.DependencyInjection.ServiceProviderAttribute",
                static (node, _) => node is ClassDeclarationSyntax,
                static (ctx, _) => (ClassDeclarationSyntax)ctx.TargetNode
            );

        // TODO: pass the pipeline to RegisterSourceOutput
    }
}
```

现在我们以经找到了我们需要的类型，接下来我们要做的就是生成代码了。我们将在下一篇文章中讲解如何生成代码。并且对用户声明的`ServiceProvider`类型进行一些检查。