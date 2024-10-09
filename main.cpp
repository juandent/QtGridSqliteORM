#include <QtWidgets/QApplication>
#include "QtWidgetsApplication1.h"
//#include "VectorModel.h"
#include "Data.h"

//using namespace std::chrono;
import date_binding;


//sys_days getToday()
//{
//    auto local_now = current_zone()->to_local(system_clock::now());
//    year_month_day ymd{ floor<days>(local_now) };
//    sys_days today = ymd;
//    return today;
//}

void prepareDB()
{
    storage().remove_all<Claim>();
    std::chrono::sys_days tp = today();
    std::chrono::year_month_day ymd = tp;
    ymd += std::chrono::months(1);
    std::chrono::sys_days sd = ymd;
    ymd += std::chrono::months(1);
    std::chrono::sys_days sdd = ymd;
    ymd += std::chrono::months(1);
    std::chrono::sys_days sde = ymd;

    Claim claim{ -1,-1,-1, -1,  tp, sd, "ASPROSE565788", "CASENo1", 2000.00, "Mil", 0, "comment" };
    storage().insert(claim);
    storage().insert(Claim{ -1,-1,-1,-1, tp,sd, "ASPROSE78889", "CASENo2", 1500, "XTTRE2",0, "comment 2", std::nullopt, std::nullopt, std::nullopt });
    storage().insert(Claim{ -1,-1,-1,-1, tp,sdd, "ASPROSE8889544", "CASENo3", 2500, "XTTRE3",0, "comment 3", std::nullopt, std::nullopt, std::nullopt });
    storage().insert(Claim{ -1,-1,-1,-1, tp,sdd, "ASPROSE95441121", "CASENo4", 3500, "XTTRE4",0, "comment 4", std::nullopt, std::nullopt, std::nullopt });
    storage().insert(Claim{ -1,-1,-1,-1, tp,sde, "ASPROSE441121889", "CASENo5", 4500, "XTTRE5",0, "comment 5", std::nullopt, std::nullopt, std::nullopt });

    std::vector<Claim> vec = storage().get_all<Claim>();
}


int main(int argc, char *argv[])
{
    prepareDB();
    QApplication a(argc, argv);
    QtWidgetsApplication1 w;
    w.show();




    return a.exec();
}
