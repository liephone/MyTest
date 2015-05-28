// 입력값
CK_SESSION_HANDLE hSesion; /* 토큰과의 세션 핸들 (2. 단계에서 획득) */
// 출력값 (인증서, keyID)
CK_BYTE pOutCert[3072], pOutKeyId[32];
CK_ULONG lOutCertLen = 0, lOutKeyIdLen = 0;
//------------------------------------------
// 변수 선언
CK_RV nRv = CKR_OK; /* 에러 코드 확인 */
Unsigned long i = 0;
CK_OBJECT_HANDLE hCertAry[15]; /* 인증서 객체 핸들 목록 */
Unsigned long ulCertCnt = 0; /* 인증서 개수 */

/* 인증서 검색을 위한 템플릿 선언 */
CK_OBJECT_CLASS certObject = CKO_CERTIFICATE;
CK_CERTIFICATE_TYPE x509Cert = CKC_X_509;
CK_ATTRIBUTE pX509CertTemplate[] = { 
	{CKA_CLASS, &certObject, sizeof(certObject)},
	{CKA_CERTIFICATE_TYPE, &x509Cert, sizeof(x509Cert)}
};

/* 인증서, keyID 획득을 위한 템플릿 선언 */
// 토큰이 꽂혀있는 슬롯 목록 획득
if (nRv = C_GetSlotList(TRUE, pSlotList, &ulSlotCnt) != CKR_OK)
goto FINISH;
// 각 슬롯의 토큰 정보를 확인하여 사용할 토큰 선택
for (i=0; i<ulSlotCnt; i++)
{
if (nRv = C_GetTokenInfo(pSlotList[i], &tokenInfo) != CKR_OK)
goto FINISH;
 // 획득한 토큰 정보 출력
}
// 사용하고자 하는 토큰 선택 (index)
// 세션 열기
nRv = C_OpenSesion(pSlotList[index], CKF_RW_SESSION | CKF_SERIAL_SESSION, &pAplication, NULL_PTR, &hSesion);
goto FINISH;
FINISH:
C_Finalize(NULL_PTR);
return nRv;
//4. 토큰에 있는 인증서 목록 중 사용하고자 하는 인증서 선택KCAC.TS.HSMU v2.30
BYTE pCertVal[220] = {0x00, };
BYTE pKeyId[20] = {0x0, };
CK_ATTRIBUTE pValueTemplate[] = { 
{CKA_VALUE, pCertVal, 0}, 
{CKA_ID, pKeyId, 0}};
// 인증서 객체를 찾기 위한 초기화
if (nRv = C_FindObjectsInit(hSesion, pX509CertTemplate, 2) != CKR_OK)
goto FINISH;
if (nRv = C_FindObjects(hSesion, hCertAry, 15, &ulCertCnt) != CKR_OK)
goto FINISH;
for (i=0; i<ulCount; i++)
{
// 템플릿 초기화
pValueTemplate[0].ulValueLen = sizeof(pCertVal);
if (nRv = C_GetAtributeValue(hSesion, hCertAry[i], pValueTemplate, 1) != 
CKR_OK)
goto FINISH;
// 획득한 인증서 사용자 선택을 위해 보여줌
// 인증서 : pValueTemplate[0].pValue, pValueTemplate[0].ulValueLen
}
// 사용하고자 하는 인증서 선택 (index)
// 선택된 인증서와 keyId 리턴
// keyID는 인증서와 쌍인 개인키, 공개키, 랜덤값을 찾는데 사용됨. pValueTemplate[0].ulValueLen = sizeof(pCertVal);
pValueTemplate[1].ulValueLen = sizeof(pKeyId);
if (nRv = C_GetAtributeValue(hSesion, hCertAry[index], pValueTemplate, 2) != 
CKR_OK)
goto FINISH;
memcpy(pOutCert, pValueTemplate[0].pValue, pValueTemplate[0].ulValueLen);
lOutCertLen = pValueTemplate[0].ulValueLen;
memcpy(pOutKeyId, pValueTemplate[1].pValue, pValueTemplate[1].ulValueLen);
lOutKeyIdLen = pValueTemplate[1].ulValueLen;
FINISH:
C_FindObjectsFinal(hSesion);
return nRv;
