
/// \file ref_counted_test.cpp
/// Contains unit tests for ref_counted related classes.

#include "dbgfmt/ref_counted.hpp"
#include <functional>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>


namespace dbgfmt::test {


class mock_ref_counted: virtual public ref_counted, public ref_counted_impl {
public:
    virtual ~mock_ref_counted() {
        if (del_handler_) {
            del_handler_();
        } else {
            assert(false && "No delete handler");
        }
    }

    void set_del_handler(const std::function<void()> & h) {
        del_handler_ = h;
    }

private:
    std::function<void()> del_handler_;
};


BOOST_AUTO_TEST_SUITE(ref_counted_test)


/// Tests constructing invalid reference
BOOST_AUTO_TEST_CASE(construct_invalid) {
    ref_counted_ref<mock_ref_counted> ref;
}


/// Tests constructing intrusive ptr
BOOST_AUTO_TEST_CASE(construct) {
    auto obj = std::make_unique<mock_ref_counted>();
    bool del_called = false;

    {
        auto old_obj = obj.get();
        obj->add_ref();
        auto ref = acquire_ref_counted(obj.release());
        BOOST_CHECK(ref);

        ref->set_del_handler([&del_called] {
            del_called = true;
        });
    }

    BOOST_CHECK(del_called);
}


/// Tests 2 references
BOOST_AUTO_TEST_CASE(second_ref) {
    auto obj = std::make_unique<mock_ref_counted>();
    bool del_called = false;

    obj->set_del_handler([&del_called] {
        del_called = true;
    });

    {
        obj->add_ref();
        auto ref = acquire_ref_counted(obj.release());
        BOOST_CHECK(ref);
        BOOST_CHECK(!del_called);

        {
            auto ref2 = ref;
        }

        BOOST_CHECK(!del_called);
    }

    BOOST_CHECK(del_called);
}



class MyRefMixin: public ref_counted_ref_base<mock_ref_counted> {

};


/// Tests reference mixin
/// Tests constructing intrusive ptr
BOOST_AUTO_TEST_CASE(mixin) {
    auto obj = std::make_unique<mock_ref_counted>();
    bool del_called = false;

    {
        auto old_obj = obj.get();
        obj->add_ref();
        auto ref = acquire_ref_counted(obj.release());
        BOOST_CHECK(ref);

        ref->set_del_handler([&del_called] {
            del_called = true;
        });
    }

    BOOST_CHECK(del_called);
}



BOOST_AUTO_TEST_SUITE_END()


}
