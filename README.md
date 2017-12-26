Using example:

```cpp
// The strings to indecates log level in human style. These values can be read from file.
static constexpr const char* const str_error = "error";
static constexpr const char* const str_warning = "warning";
static constexpr const char* const str_info = "info";
static constexpr const char* const str_debug = "debug";

// The machine style of log levels.
enum class Level
{
  error,
  warning,
  info,
  debug
};

Level level;

void setLevel(srd::string& level)
{
  /* This function attachs each machine style value with human style.
   * No switch-case and if/else constructions are required.
   * This construction can be easy expanded if new log level is required.
   */
  at_tuple(
    [&lv](auto& item)
    {
      return (item.second == lv);
    },
    [&level](auto& item)
    {
      level = item.first;
    },
    std::make_pair(Level::error, str_error),
    std::make_pair(Level::warning, str_warning),
    std::make_pair(Level::info, str_info),
    std::make_pair(Level::debug, str_debug)
  );
}

// Set log level as warning.
setLevel(str_warning);
```
