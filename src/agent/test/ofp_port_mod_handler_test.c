/*
 * Copyright 2014-2015 Nippon Telegraph and Telephone Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "unity.h"
#include "../ofp_port_mod_handler.h"
#include "handler_test_utils.h"
#include "event.h"
#include "lagopus/pbuf.h"


void
setUp(void) {
}

void
tearDown(void) {
}

lagopus_result_t
ofp_port_mod_handle_wrap(struct channel *channel,
                         struct pbuf *pbuf,
                         struct ofp_header *xid_header,
                         struct ofp_error *error) {
  return ofp_port_mod_handle(channel, pbuf,
                             xid_header, error);
}

void
test_ofp_port_mod_handle_normal_pattern(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  ret = check_packet_parse(
          ofp_port_mod_handle_wrap,
          "04 10 00 28 00 00 00 10 00 00 00 00 00 00 00 00 "
          /* <---------------------------------------------... ofp_port_mod
           * <---------------------> ofp_header
           * <> version
           *    <> type
           *       <---> lenght = 5 * 8 bytes
           *             <--------> xid
           *                         <---------> port_no
           *                                     <---------> padding
           */
          "ff ff ff ff ff ff 00 00 00 00 00 24 00 00 00 24 "
          /* ...-------------------------------------------... ofp_port_mod
           * <---------------> hw_addr
           *                   <---> padding
           *                         <---------> config
           *                                     <---------> mask
           */
          "00 00 00 05 00 00 00 00");
  /* ...-------------------> ofp_port_mod
   * <---------> advertise ( OFPPF_10MB_HD | OFPPF_100MB_HD)
   *             <--------> padding
   */

  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OK, ret,
                            "ofp_port_mod_handle(normal) error.");
}

void
test_ofp_port_mod_handle_invalid_length(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* header only */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 08 00 00 00 10",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "no body error.");
}

void
test_ofp_port_mod_handle_invalid_length1(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no pad, hw_addr, pad2, config, mask, advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 0c 00 00 00 10 00 00 00 01",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length2(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no hw_addr, pad2, config, mask, advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 10 00 00 00 10 00 00 00 01 00 00 00 00",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length3(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no pad2, config, mask, advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 16 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length4(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no config, mask, advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 18 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff 00 00",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length5(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no mask, advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 1c 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 00 00 00 03",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length6(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no advertise, pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 20 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 00 00 00 24 00 00 00 04",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length7(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* no pad3 */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 24 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 00 00 00 24 00 00 00 04 "
          "00 00 00 05",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_length8(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  /* over size */
  ofp_error_set(&expected_error, OFPET_BAD_REQUEST, OFPBRC_BAD_LEN);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 2c 00 00 00 10 00 00 00 01 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 00 00 00 24 00 00 00 04 "
          "00 00 00 05 00 00 00 00 ff ff ff ff",
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid body error.");
}

void
test_ofp_port_mod_handle_invalid_config(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  ofp_error_set(&expected_error, OFPET_PORT_MOD_FAILED, OFPPMFC_BAD_CONFIG);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 28 00 00 00 10 00 00 00 00 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 80 00 00 00 ff ff ff ff "
          /* ...-------------------------------------------... ofp_port_mod
           * <---------------> hw_addr
           *                   <---> padding
           *                         <---------> config (1 << 31 invalid ofp_port_config
           *                                     <---------> mask
           */
          "00 00 00 05 00 00 00 00",
          /* ...-------------------> ofp_port_mod
           * <---------> advertise ( OFPPF_10MB_HD | OFPPF_100MB_HD)
           *             <--------> padding
           */
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid config should cause error");
}

void
test_ofp_port_mod_handle_invalid_advertise(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct ofp_error expected_error = {0, 0, {NULL}};
  ofp_error_set(&expected_error, OFPET_PORT_MOD_FAILED, OFPPMFC_BAD_ADVERTISE);
  ret = check_packet_parse_expect_error(
          ofp_port_mod_handle_wrap,
          "04 10 00 28 00 00 00 10 00 00 00 00 00 00 00 00 "
          "ff ff ff ff ff ff 00 00 00 00 00 24 00 00 00 24 "
          "80 00 00 00 00 00 00 00",
          /* ...-------------------> ofp_port_mod
           * <---------> advertise ( 1 << 31 is invalid ofp_port_features
           *             <--------> padding
           */
          &expected_error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_OFP_ERROR, ret,
                            "invalid advertise should cause error");
}

void
test_ofp_port_mod_handle_null(void) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  struct event_manager *em = event_manager_alloc();
  struct channel *channel = channel_alloc_ip4addr("127.0.0.1", "1000",
                            em, 0x01);
  struct pbuf *pbuf = pbuf_alloc(65535);
  struct ofp_header xid_header;
  struct ofp_error error;

  ret = ofp_port_mod_handle(NULL, pbuf, &xid_header, &error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_INVALID_ARGS, ret,
                            "NULL-check error. (channel)");

  ret = ofp_port_mod_handle(channel, NULL, &xid_header, &error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_INVALID_ARGS, ret,
                            "NULL-check error. (pbuf)");

  ret = ofp_port_mod_handle(channel, pbuf, NULL, &error);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_INVALID_ARGS, ret,
                            "NULL-check error. (xid_header)");

  ret = ofp_port_mod_handle(channel, pbuf, &xid_header, NULL);
  TEST_ASSERT_EQUAL_MESSAGE(LAGOPUS_RESULT_INVALID_ARGS, ret,
                            "NULL-check error. (ofp_error)");
  channel_free(channel);
  pbuf_free(pbuf);
  event_manager_free(em);
}
