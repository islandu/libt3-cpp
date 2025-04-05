#include "pinkytoe/enums.h"
#include "pinkytoe/move_ledger.h"
#include "meta.hpp"
#include "player.hpp"

namespace pinkytoe
{

constexpr MoveLedger::MoveLedger(Player start_player) noexcept
  : start_player_(start_player)
{
}

/// @brief Records next move
/// @param r Row index
/// @param c Column index
constexpr void MoveLedger::record_next(std::uint8_t r, std::uint8_t c) noexcept
{
  using impl::bool_as_integral;
  using impl::enum_as_integral;
  using impl::whose_turn;

  // Update balances
  auto move_count = this->history_.move_count();
  auto player = whose_turn(this->start_player_, move_count);
  auto p = enum_as_integral(player);

  this->r_bals_[r] += p;
  this->c_bals_[c] += p;

  auto on_diag0 = bool_as_integral<std::int8_t>(r == c);
  auto on_diag1 = bool_as_integral<std::int8_t>(r + c == 2);
  this->d_bals_[0] += on_diag0 * p;
  this->d_bals_[1] += on_diag1 * p;

  // Add move to history
  this->history_.push(r, c);
}

/// @brief Removes last record from ledger
constexpr void MoveLedger::remove_last() noexcept
{
  using impl::bool_as_integral;
  using impl::enum_as_integral;
  using impl::whose_turn;

  // Pop last move from history
  std::uint8_t r{}, c{};
  this->history_.pop(r, c);

  auto move_count = this->history_.move_count();
  auto player = whose_turn(this->start_player_, move_count);
  auto p = enum_as_integral(player);

  // Rollback balances
  this->r_bals_[r] -= p;
  this->c_bals_[c] -= p;

  auto on_diag0 = bool_as_integral<std::int8_t>(r == c);
  auto on_diag1 = bool_as_integral<std::int8_t>(r + c == 2);
  this->d_bals_[0] -= on_diag0 * p;
  this->d_bals_[1] -= on_diag1 * p;
}

/// @brief Removes last record from ledger
/// @param r Stores removed row index
/// @param c Stores removed row index
constexpr void remove_last(std::uint8_t& r, std::uint8_t& c) noexcept
{

}

/// @brief Returns number of moves currently recorded
/// @return Number of recorded moves
constexpr std::uint8_t MoveLedger::move_count() noexcept
{
  return this->history_.move_count();
}

/// @brief Returns the player configured to go first
/// @return The player the moves/moved first
constexpr Player MoveLedger::first_player() noexcept
{
  return this->start_player_;
}

} // namespace pinkytoe
