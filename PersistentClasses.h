#pragma once

#include<chrono>
#include <string>
#include <optional>

struct Claim
{
	int id;
	int fkey_patient;
	int fkey_doctor;
	int fkey_medication;					// for chronic meds
	std::chrono::sys_days start_date;
	std::chrono::sys_days submission_date;
	std::string asprose_claim_number;				// Claim: 85481
	std::string asprose_case_number;		// Case : 21INSENE0085
	double asprose_amount_presented;		// in colones
	std::string ins_claim_number;			// 202001017481-ACCSA0120094110
	int asprose_acknowledgement_type;		// full, partial, none: TODO: create an enum instead
	std::string comment;
	std::optional<double> amount;
	std::optional<int> other_system_id;
	std::optional<std::vector<char>>  photo;

	//std::string simple_dump() const
	//{
	//	std::string str = std::to_string(id) + " - "s + util::to_string(start_date) + " - "s + util::to_string(submission_date) + " " + std::to_string(fkey_patient);
	//	return str;
	//}

	//std::string dump() const;

	//double get_total_amount()
};
