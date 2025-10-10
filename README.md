# Winter

> **Unreal Engine 4, C++**, BP

### **개발 인원** 
3명 (클라이언트 2명, 서버 1명)

### **개발 기간**
- 2025.04.11 ~ 2025.09.04

### **게임 소개**
1. 게임이 시작되기 전 플레이어가 게임에 접속하면 자신이 조종할 곰 캐릭터가 지정되며 플레이어가 접속할 때마다 곰 캐릭터가 모닥불 주위로 둘러서 소환된다.
2. 게임이 시작되면 플레이어들은 월드 내에 고르게 퍼져있는 눈무더기에서 눈덩이를 파밍하여 눈덩이를 던져서 플레이어를 공격할 수 있다.
3. 곰 캐릭터에게는 체온이라는 체력시스템이 있으며 체온이 30도 이하로 떨어지면 동상에 걸려 속도가 느려지며, 27도에 도달하면 캐릭터의 외형이 눈사람으로 전환된다.
4. 눈사람이 된 캐릭터가 곰 캐릭터에 가까이 붙으면 서로의 상태가 전환되어 곰 캐릭터는 눈사람 캐릭터가 되고 눈사람 캐릭터는 곰 캐릭터가 된다.
5. 일반 아이템은 아이템 상자에서 얻을 수 있으며, 일정 시간이 지났을 때 보급아이템이 하늘에서 떨어지고 특수 아이템을 획득할 수 있다.
6. 한 명의 플레이어를 제외한 모든 플레이어가 눈사람 캐릭터로 변한 경우 게임이 종료되며 혼자 살아남은 플레이어는 이 게임의 승자가 된다.

<체온이 감소하는 경우>
- 모닥불 지역 밖으로 나가는 경우 서서히 감소
- 다른 플레이어가 던진 눈덩이나 공격아이템으로 피격될 경우

<체온이 증가하는 경우>
- 모닥불 지역으로 들어오는 경우 서서히 회복
- 성냥 아이템을 사용할 경우

<일반 아이템>
- 우산 : 눈덩이 공격을 막을 수 있다.
- 가방 : 눈덩이를 더 많이 저장할 수 있다.
- 성냥 : 즉시 체온을 회복한다.

<특수 아이템>
- 샷건 : 눈덩이보다 더 넓은 범위의 공격을 한다.
- 얼음 장벽 : 바닥에 던지면 얼음장벽이 세워지며 공격을 방어할 수 있다.
- 제트 스키 : 제트 스키에 탑승할 수 있으며 빠르게 이동한다.

<이벤트 요소>
- 플레이어가 토네이도에 들어가면 캐릭터가 회전하면서 위로 솟구치고 임의의 방향으로 날려보내진다.
- 발판을 통해 높게 점프하면서 도망칠 수 있다. 발판은 곰 캐릭터만 사용할 수 있고 눈사람 캐릭터는 사용하지 못한다.

### **Youtube**
[![Watch the demo](https://img.youtube.com/vi/ja5gYuHjb-U/maxresdefault.jpg)](https://youtu.be/ja5gYuHjb-U?si=9OJIB3y_7rJcWNw6)

### 구현 기능(담당)

<details>
  <summary> 레벨 디자인</summary>

<img width="1280" height="720" alt="슬라이드31" src="https://github.com/user-attachments/assets/6daa972c-e757-468b-a023-79ba2434279b" />
</details>

<details>
  <summary> VFX 제작</summary>
  
<img width="1280" height="720" alt="슬라이드32" src="https://github.com/user-attachments/assets/95ba3892-1da8-4ecc-b341-d8a0b87345b7" />

</details>

<details>
  <summary> 상태 동기화</summary>
  

<img width="1280" height="720" alt="슬라이드33" src="https://github.com/user-attachments/assets/e19e1c98-c3f3-4957-bd5b-e15ec5e38c22" />
<img width="1280" height="720" alt="슬라이드34" src="https://github.com/user-attachments/assets/6c3cc06e-a9f0-40dc-bfe6-50a683e814e7" />
<img width="1280" height="720" alt="슬라이드35" src="https://github.com/user-attachments/assets/98ae84ea-8cbf-40e6-8f87-ecb3a1124230" />
<img width="1280" height="720" alt="슬라이드36" src="https://github.com/user-attachments/assets/e103bc52-d43d-412b-b666-b272102597c3" />

</details>

<details>
  <summary> 후디니와 언리얼의 연동</summary>

<img width="1280" height="720" alt="슬라이드37" src="https://github.com/user-attachments/assets/a2ce4087-97ed-49f3-a9a7-6f6a7cf36aaf" />
<img width="1280" height="720" alt="슬라이드39" src="https://github.com/user-attachments/assets/329afdcd-b682-40e6-b1bf-fb87f9c4eb29" />

</details>

<details>
  <summary> 캐릭터 부위 판정</summary>
  
<img width="1280" height="720" alt="슬라이드38" src="https://github.com/user-attachments/assets/14835872-09ec-4faa-b4ff-04d3a5158694" />

</details>

<details>
  <summary> 카메라 이동</summary>

 - 로비 카메라에서 플레이어의 카메라로 부드럽게 전환
<img width="1280" height="720" alt="2022 한국공학대학교 게임공학과 졸업작품 WINTER 소개 영상 0-9 screenshot" src="https://github.com/user-attachments/assets/ee77d9fd-36c0-400c-9980-029c4b286c67" />
<img width="1280" height="720" alt="2022 한국공학대학교 게임공학과 졸업작품 WINTER 소개 영상 0-10 screenshot" src="https://github.com/user-attachments/assets/fd872b10-e9cc-4f9f-b543-f0a03ee6743b" />

</details>

<details>
  <summary> 충돌과 눈덩이 데미지 설정, 눈사람과 캐릭터 사이 상태 전환</summary>

 - 곰 캐릭터의 체온이 0이 되면 눈사람으로 바뀐다.
<img width="1280" height="720" alt="2022 한국공학대학교 게임공학과 졸업작품 WINTER 소개 영상 3-56 screenshot" src="https://github.com/user-attachments/assets/166447a7-84d8-4740-83f5-f741416688d7" />

 - 곰 캐릭터와 눈사람 캐릭터가 접촉하면 서로의 상태가 뒤바뀐다.
<img width="1280" height="720" alt="2022 한국공학대학교 게임공학과 졸업작품 WINTER 소개 영상 3-56 screenshot" src="https://github.com/user-attachments/assets/3f7b9f90-050a-4c85-a791-7b821983a188" />

</details>
