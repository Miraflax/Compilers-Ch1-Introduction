#include "slp.h"
#include "util.h"

AStm prog(void) {

    return
        make_ACompoundStm(
                make_AAssignStm(
                    "a",
                    make_AOpExp(
                        make_ANumExp(5),
                        A_PLUS,
                        make_ANumExp(3)
                        )
                    ),
                make_ACompoundStm(
                    make_AAssignStm(
                        "b",
                        make_AEseqExp(
                            make_APrintStm(
                                make_APairExpList(
                                    make_AIdExp("a"),
                                    make_ALastExpList(
                                        make_AOpExp(
                                            make_AIdExp("a"),
                                            A_MINUS, 
                                            make_ANumExp(1)
                                            )
                                        )
                                    )
                                ),
                            make_AOpExp(
                                make_ANumExp(10),
                                A_TIMES,
                                make_AIdExp("a")
                                )
                            )
                            ),
                        make_APrintStm(make_ALastExpList(
                                    make_AIdExp(
                                        "b")
                                    )
                                )
                            )
                            );
}
