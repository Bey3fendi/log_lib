namespace log_daemon {
class App {
public:
  App() = default;
  App(const App &) = delete;
  App(const App &&) = delete;
  virtual ~App() = default;
  App &operator=(const App &) = delete;
  App &operator=(const App &&) = delete;

  bool Init();

  bool Start();

  bool Stop();

private:
};
} // namespace log_daemon