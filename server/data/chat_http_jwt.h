//
// Created by lambert on 23-6-3.
//

#pragma once

#include <optional>
#include "DataFacade.h"

inline void RandomGenerateJwtSecret() {
    std::string salt;
    Poco::RandomInputStream rng;
    for (size_t i = 0; i < 24; ++i) {
        char c;
        rng >> c;
        salt += c;
    }
    std::stringstream ss;
    Poco::Base64Encoder base_64_encoder(ss);
    base_64_encoder << salt;
    base_64_encoder.close();  // 关闭编码器以刷新任何缓冲的数据
    g_JWT_secret = ss.str();
}

inline std::string SetJwt(const int &user_id_, const std::string &username) {
    int user_id;
    if (user_id_ == 0) {
        user_id = DataFacade::get_id_from_name(username);
    }
    Poco::JWT::Signer signer{g_JWT_secret};
    Poco::JWT::Token token;
    token.setIssuer("charServerLambert");
    token.payload().set("username", username);
    token.payload().set("user_id", user_id);
    token.setIssuedAt(Poco::Timestamp());
    token.setNotBefore(Poco::Timestamp());
    token.setExpiration(Poco::Timestamp() + Poco::Timespan(0, 1, 0, 0, 0));
    string jwt = signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
    return jwt;
}

inline std::optional<std::tuple<int, string>> VerifyJwt(Poco::Net::HTTPServerRequest &request,
                                                        Poco::JSON::Object::Ptr &response_object) {
    /// If verify successfully, verify shouldn't change the response json;
    std::function<std::optional<std::tuple<int, string>>()> SetFail = [&response_object]() -> std::optional<std::tuple<int, string>> {
        response_object = new Poco::JSON::Object();
        response_object->set("code", static_cast<int>(state_code::VERIFY_TOKEN_FAILED));
        response_object->set("msg", "JWT验证失败");
        return std::nullopt;
    };

    if (!request.has("Authorization")) {
        return SetFail();
    }
    string jwt = request.get("Authorization", "");
    if (jwt.size() < 7)
        return SetFail();
    jwt = jwt.substr(7);
    try {
        Poco::JWT::Signer signer{g_JWT_secret};
        Poco::JWT::Token token = signer.verify(jwt);
        int id = token.payload().get("user_id");
        Poco::Dynamic::Var user_name = token.payload().get("username");
        return std::make_tuple(id, user_name.toString());
    }
    catch (Poco::Exception &e) {
        return SetFail();
    }
}