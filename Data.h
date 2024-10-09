#pragma once

#include <sqlite_orm/sqlite_orm.h>

#include <chrono>

import date_binding;

//#include "date_binding.h"

#include "PersistentClasses.h"



inline auto& storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	const string db_name = "database.sqlite";
	static int flag = 0;

	static auto _storage =
				make_storage(db_name,
					make_table("Claims",
						make_column("id_claim", &Claim::id, primary_key().autoincrement()),
						make_column("fkey_patient", &Claim::fkey_patient),
						make_column("fkey_doctor", &Claim::fkey_doctor),
						make_column("fkey_medication", &Claim::fkey_medication),
						make_column("start_date", &Claim::start_date),
						make_column("submission_date", &Claim::submission_date),
						make_column("asprose_claim_number", &Claim::asprose_claim_number),
						make_column("asprose_case_number", &Claim::asprose_case_number),
						make_column("asprose_amount_presented", &Claim::asprose_amount_presented),
						make_column("ins_claim_number", &Claim::ins_claim_number),
						make_column("asprose_acknowledgement_type", &Claim::asprose_acknowledgement_type),
						make_column("comment", &Claim::comment),
						make_column("amount", &Claim::amount),
						make_column("other_system_id", &Claim::other_system_id),
						make_column("photo", &Claim::photo)
			));

	if(flag == 0)
	{
		flag = 1;
		_storage.sync_schema(true);
	}

	return _storage;
}
