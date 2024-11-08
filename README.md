# my_muduo
### 1. **TcpServer 的初始化**

   - `TcpServer` 是整个服务器的核心组件，负责处理客户端连接请求。
   - 初始化 `TcpServer` 对象时，会创建一个 `Acceptor`，它包含一个监听文件描述符（`listenfd`）和一个 `acceptChannel`，用于监听新连接事件。
   - 通过 `setNewConnectionCallback` 将 `TcpServer::newConnection` 方法设置为 `Acceptor` 的回调函数，当有新连接到来时，触发此回调。

### 2. **主线程的 EventLoop (mainLoop)**

   - `mainLoop` 是主线程中的 `EventLoop` 对象，负责监听 `Acceptor` 的事件。
   - `TcpServer::start` 方法会启动 `EventLoopThreadPool`，创建多个子线程，每个子线程运行一个独立的 `EventLoop`，称为 `subLoop`。
   - 主线程的 `mainLoop` 主要负责监听新的连接请求，而客户端的具体数据处理会分配给 `subLoop`。

### 3. **EventLoopThreadPool 的工作机制**

   - `EventLoopThreadPool` 是一个线程池，用于管理多个 `EventLoop` 线程。
   - 通过 `start` 方法启动 `EventLoopThreadPool`，创建 `numThreads_` 个线程，每个线程都运行一个 `subLoop`。
   - 每个 `subLoop` 都是一个独立的 `EventLoop`，和主线程的 `mainLoop` 平行工作，用于处理客户端的 I/O 事件。

### 4. **Acceptor 的事件处理**

   - `Acceptor` 的作用是接受新的客户端连接。
   - 它在主线程的 `mainLoop` 中监听 `listenfd` 的读事件，一旦有新的连接请求到来，就会触发 `acceptChannel` 的读回调，即 `handleRead()` 方法。
   - `handleRead()` 方法会调用 `newConnectionCallback`，即 `TcpServer::newConnection`，来处理新的连接。

### 5. **TcpServer::newConnection**：处理新连接

   - 在 `newConnection` 中，`TcpServer` 创建一个新的 `TcpConnection` 对象来表示客户端连接。
   - 然后，`TcpServer` 使用轮询算法选择一个 `subLoop`（从多个子线程的 `EventLoop` 中选择一个），将新的连接分配给该 `subLoop`。
   - 具体步骤：
     - 根据轮询算法选择 `subLoop`，将新的 `connfd` 转换为一个 `channel`。
     - 将 `TcpConnection` 的回调函数注册到 `subLoop` 上，以便在该子线程中处理客户端的事件。

### 6. **Reactor 模式的实现**（每个线程一个 `Reactor`）

   - 每个 `EventLoop` 充当一个 `Reactor`，负责在其线程中管理和处理所有事件。
   - 主线程的 `mainLoop` 只负责新的连接请求的监听，而所有的客户端 I/O 事件都会在 `subLoop` 中处理。
   - 每个 `subLoop` 都包含一个 `EPollPoller`，用于监听事件，通过 `poll()` 方法轮询事件并执行相应的回调。

### 7. **线程间的唤醒机制**

   - 当主线程将新的连接分配给某个 `subLoop` 时，需要唤醒该子线程，以便立即处理新的连接。
   - 通过 `eventfd` 或 `socketpair` 实现线程间的唤醒机制。
   - 每个 `subLoop` 线程都有一个 `wakeUpFd`，主线程通过写入 `wakeUpFd` 唤醒子线程。

### 8. **TcpConnection 的回调处理**

   - `TcpConnection` 表示一个客户端连接，负责具体的读写数据操作。
   - 它包含了两个主要的回调函数：
     - `onConnection`：当连接建立时触发，业务逻辑可以选择在这里处理一些连接初始化工作。
     - `onMessage`：当有数据到达时触发，用于读取和处理客户端发送的数据。
   - 这些回调会被注册到 `subLoop` 中，在 `subLoop` 监听到客户端事件后，自动执行相应的回调函数。

### 9. **数据发送和接收** 

   - 当客户端发送数据时，`onMessage` 回调被触发，`TcpConnection` 会从 `connfd` 中读取数据并进行处理。
   - 当服务器需要发送数据给客户端时，`TcpConnection` 负责将数据写入 `connfd`，并确保数据被正确发送给客户端。

### 10.定时器
![image](https://github.com/user-attachments/assets/024c336a-ff30-4a54-a2fd-bdd291bb9375)

