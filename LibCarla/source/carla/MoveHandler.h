// 版权 © 2019 自由大学计算机视觉中心（CVC）
// de Barcelona (UAB)[巴塞罗那（UAB].
//
// 本作品采用 MIT 许可证授权e.
// 欲查看许可证副本，请访问 https://opensource.org/licenses/MIT.

// 这段代码的目的是为了解决某些库（如 Asio）不能正确处理仅可移动对象的问题。
//通过 MoveWrapper，代码可以确保传入的处理程序对象在 Asio 等库中能够安全地进行移动而非复制，从而避免可能出现的编译错误
#pragma once  // 确保此头文件仅被包含一次

#include <type_traits>  // 包含类型特征相关的头文件，提供类型特性支持
#include <utility>  // 包含通用工具函数，比如 std::move

namespace carla {  // 定义命名空间 carla
namespace detail {   // 定义命名空间 detail，用于实现细节


  template <typename FunctorT>  // 定义一个模板结构体，接受任意类型 FunctorT
  struct MoveWrapper : FunctorT {  // MoveWrapper 继承自 FunctorT
    MoveWrapper(FunctorT &&f) : FunctorT(std::move(f)) {}  // 构造函数，移动构造 FunctorT

    MoveWrapper(MoveWrapper &&) = default;  // 移动构造函数，使用默认实现
    MoveWrapper& operator=(MoveWrapper &&) = default;  // 移动赋值运算符，使用默认实现

    MoveWrapper(const MoveWrapper &);  // 声明复制构造函数（未定义）
    MoveWrapper& operator=(const MoveWrapper &);  // 声明复制赋值运算符（未定义）
  };

} // namespace detail

  /// 下面是一种hack，用于绕过Asio库的限制，使其能够接受仅可移动的处理程序。
  /// 如果Asio试图复制一个仅可移动的处理程序，那么编译时可能不会立即报错
  ///
  /// @see https://stackoverflow.com/a/22891509.
  template <typename FunctorT>  // 定义一个模板函数，接受任意类型 FunctorT
  auto MoveHandler(FunctorT &&func) {   // MoveHandler 函数返回一个移动处理器
    using F = typename std::decay<FunctorT>::type;  // 使用 std::decay 处理 FunctorT 的类型
    return detail::MoveWrapper<F>{std::move(func)};   // 返回一个 MoveWrapper 实例，移动传入的 func
  }

} // namespace carla
