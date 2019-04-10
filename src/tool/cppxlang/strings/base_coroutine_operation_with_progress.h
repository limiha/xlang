
namespace std::experimental
{
    template <typename TResult, typename TProgress, typename... Args>
    struct coroutine_traits<xlang::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>, Args...>
    {
        struct promise_type final : xlang::impl::promise_base<promise_type,
            xlang::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>,
            xlang::Windows::Foundation::AsyncOperationWithProgressCompletedHandler<TResult, TProgress>, TProgress>
        {
            using ProgressHandler = xlang::Windows::Foundation::AsyncOperationProgressHandler<TResult, TProgress>;

            void Progress(ProgressHandler const& handler) noexcept
            {
                std::lock_guard const guard(this->m_lock);
                m_progress = handler;
            }

            ProgressHandler Progress() noexcept
            {
                std::lock_guard const guard(this->m_lock);
                return m_progress;
            }

            TResult get_return_value() noexcept
            {
                return std::move(m_result);
            }

            void return_value(TResult&& value) noexcept
            {
                m_result = std::move(value);
            }

            void return_value(TResult const& value) noexcept
            {
                m_result = value;
            }

            void set_progress(TProgress const& result)
            {
                if (auto handler = Progress())
                {
                    handler(*this, result);
                }
            }

            TResult m_result{ xlang::impl::empty_value<TResult>() };
            ProgressHandler m_progress;
        };
    };
}