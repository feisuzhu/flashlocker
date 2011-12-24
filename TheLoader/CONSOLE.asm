;MASMPlus 代码模板 - 控制台程序

.386
.mmx

.model flat, stdcall
option casemap :none

SECTION_INFO STRUCT
	dwRVA dd ?
	dwSize dd ?
SECTION_INFO ENDS

ENCINFO STRUCT
	dwOverlayOffset dd ?
	dwOverlaySize dd ?
	dwTheSeed dd ?
ENCINFO ENDS

PRGINFO STRUCT
	dwEntryPoint dd ?
	dwImport dd ?
	dwSections dd ?
	dwIsBoundImport dd ?
PRGINFO ENDS

KEYSTRUCT STRUCT
	dwSignature dd ?
	dwSeed dd ?
	ddqTheKey dd 4 dup(?)
KEYSTRUCT ENDS

include windows.inc

includelib kernel32.lib
includelib user32.lib


DlgProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
;TEAEncrypt proto lpData:DWORD,cbData:DWORD,lpKey:DWORD
TEADecrypt proto lpData:DWORD,cbData:DWORD,lpKey:DWORD

RLC MACRO VAR ;ReLoCation!
	EXITM <[ebx+(VAR-START)]>
ENDM

VirJmp MACRO WHERE
	lea eax,RLC(WHERE)
	jmp eax
	EXITM
ENDM

OFFS MACRO VAR ;OFFSET!
	EXITM <(VAR-START)>
ENDM
	
APICall MACRO FunArgs:VARARG  ; this macro is from the macro.asm
	LOCAL txt, arg,curr,addrarg,buseeax
	txt TEXTEQU <>
	buseeax = 0
	;反转参数
	%FOR arg, <FunArgs>
	  txt CATSTR <arg>, <!,>, txt
	ENDM
	txt SUBSTR  txt, 1, @SizeStr( %txt ) - 1
	;压入所有参数
	:PushNext
		curr INSTR 1,txt,<!,>
		IF curr NE 0
			arg  SUBSTR txt,1,curr-1
			txt  SUBSTR txt,curr+1,@SizeStr(% txt)-curr
			IF @SizeStr(% txt) GE 1
				curr INSTR 1,arg,<!ADDR>
				IF curr NE 0
					arg SUBSTR arg,6,@SizeStr(% arg)-5
					buseeax=1
					lea eax,arg		;使用了eax,做一个标记,如果在这之后直接使用push eax时,显示错误
					push eax
				ELSE
					IF buseeax EQ 1
						ifidn arg,<!eax>
							.err <addr overwrite eax>		;显示错误在使用eax之前使用了addr,中止编译
						else
							push arg
						endif
						;检查是否使用了eax
					ELSE
						push arg
					ENDIF
				ENDIF
				goto PushNext
			ENDIF
		ENDIF
		IF buseeax EQ 1
			ifidn txt,<!eax>
				.err <addr overwrite eax>		;显示错误在使用eax之前使用了addr,中止编译
			else
				call DWORD ptr RLC(@CatStr(<ptr>,%txt))
			endif
			;检查是否使用了eax
		ELSE
			call DWORD ptr RLC(@CatStr(<ptr>,%txt))
		ENDIF
ENDM

ProcCall MACRO FunArgs:VARARG
	LOCAL txt, arg,curr,addrarg,buseeax
	txt TEXTEQU <>
	buseeax = 0
	;反转参数
	%FOR arg, <FunArgs>
	  txt CATSTR <arg>, <!,>, txt
	ENDM
	txt SUBSTR  txt, 1, @SizeStr( %txt ) - 1
	;压入所有参数
	:PushNext
		curr INSTR 1,txt,<!,>
		IF curr NE 0
			arg  SUBSTR txt,1,curr-1
			txt  SUBSTR txt,curr+1,@SizeStr(% txt)-curr
			IF @SizeStr(% txt) GE 1
				curr INSTR 1,arg,<!ADDR>
				IF curr NE 0
					arg SUBSTR arg,6,@SizeStr(% arg)-5
					buseeax=1
					lea eax,arg		;使用了eax,做一个标记,如果在这之后直接使用push eax时,显示错误
					push eax
				ELSE
					IF buseeax EQ 1
						ifidn arg,<!eax>
							.err <addr overwrite eax>		;显示错误在使用eax之前使用了addr,中止编译
						else
							push arg
						endif
						;检查是否使用了eax
					ELSE
						push arg
					ENDIF
				ENDIF
				goto PushNext
			ENDIF
		ENDIF
		IF buseeax EQ 1
			ifidn txt,<!eax>
				.err <addr overwrite eax>		;显示错误在使用eax之前使用了addr,中止编译
			else
				call txt
			endif
			;检查是否使用了eax
		ELSE
			call txt
		ENDIF
ENDM

.CODE
START:
pushad
call @F
nop ;for align
nop

_stEncInfo ENCINFO <>

;Import Table
;IMAGE_THUNK_DESCRIPTOR
;Import ADDR: Base+1014h, Size ??

IT:
DD KERNEL32_IAT-401000h ;KERNEL32.DLL's OriginalFirstThunk
DD 0 ;TimeDateStamp
DD 0 ;ForwarderChain
DD dllKERNEL32-401000h ;The rva of "KERNEL32.DLL"
DD KERNEL32_IAT-401000h;"Pointer to _IMAGE_THUNK_DATA

DD USER32_IAT-401000h ;USER32.DLL's OriginalFirstThunk
DD 0 ;TimeDateStamp
DD 0 ;ForwarderChain
DD dllUSER32-401000h ;The rva of "USER32.DLL"
DD USER32_IAT-401000h;"Pointer to _IMAGE_THUNK_DATA

DD 5 DUP(0) ;End of import table

dllKERNEL32 db "KERNEL32.DLL",0
dllUSER32 db "USER32.DLL",0

align DWORD

KERNEL32_IAT:
ptrSetFilePointer DD funcSetFilePointer-401000h
ptrCreateFile DD funcCreateFile-401000h
ptrExitProcess DD funcExitProcess-401000h
ptrGetDriveType DD funcGetDriveType-401000h
ptrGetModuleFileName DD funcGetModuleFileName-401000h
ptrGetModuleHandle DD funcGetModuleHandle-401000h
ptrGetProcAddress DD funcGetProcAddress-401000h
ptrLoadLibrary DD funcLoadLibrary-401000h
ptrReadFile DD funcReadFile-401000h
ptrRtlMoveMemory DD funcRtlMoveMemory-401000h
ptrCloseHandle DD funcCloseHandle-401000h
ptrVirtualAlloc DD funcVirtualAlloc-401000h
ptrGetDiskFreeSpaceEx DD funcGetDiskFreeSpaceEx-401000h
ptrVirtualFree DD funcVirtualFree-401000h
DD 0

USER32_IAT:
ptrEndDialog DD funcEndDialog-401000h
ptrLoadIcon DD funcLoadIconA-401000h
ptrMessageBox DD funcMessageBoxA-401000h
ptrSendDlgItemMessage DD funcSendDlgItemMessageA-401000h
ptrSendMessage DD funcSendMessageA-401000h
DD 0

funcSetFilePointer            DB 0,0,"SetFilePointer",0
funcCreateFile                DB 0,0,"CreateFileA",0
funcExitProcess               DB 0,0,"ExitProcess",0
funcGetDriveType              DB 0,0,"GetDriveTypeA",0
funcGetModuleFileName         DB 0,0,"GetModuleFileNameA",0
funcGetModuleHandle           DB 0,0,"GetModuleHandleA",0
funcGetProcAddress            DB 0,0,"GetProcAddress",0
funcLoadLibrary               DB 0,0,"LoadLibraryA",0
funcReadFile                  DB 0,0,"ReadFile",0
funcRtlMoveMemory             DB 0,0,"RtlMoveMemory",0
funcCloseHandle               DB 0,0,"CloseHandle",0
funcVirtualAlloc              DB 0,0,"VirtualAlloc",0
funcGetDiskFreeSpaceEx        DB 0,0,"GetDiskFreeSpaceExA",0
funcVirtualFree               DB 0,0,"VirtualFree",0

funcEndDialog                 DB 0,0,"EndDialog",0
funcLoadIconA                 DB 0,0,"LoadIconA",0
funcMessageBoxA               DB 0,0,"MessageBoxA",0
funcSendDlgItemMessageA       DB 0,0,"SendDlgItemMessageA",0
funcSendMessageA              DB 0,0,"SendMessageA",0

devdrv db "\\.\"
drvstr db "C:\",0
szErrStartup db "程序初始化失败，请确认是否有其他程序占用本程序！",0
szErrNoKeyDisk db "该程序已被保护,请插入钥匙盘,否则程序无法继续运行!",0
@@:
pop ebx
sub ebx,6; Get base

MainProc proc
LOCAL buff [512]:BYTE
LOCAL junk:DWORD
LOCAL hInstance:DWORD
LOCAL stEncInfo:ENCINFO
LOCAL hMem:DWORD
LOCAL hCurDLL:DWORD
	
	APICall GetModuleHandle,NULL
	mov hInstance,eax
	lea eax,RLC(_stEncInfo)
	APICall RtlMoveMemory,ADDR stEncInfo,eax,SIZEOF stEncInfo
	lea esi,buff
	APICall GetModuleFileName,hInstance,esi,MAX_PATH
	APICall CreateFile,esi,GENERIC_READ,FILE_SHARE_READ OR FILE_SHARE_WRITE OR FILE_SHARE_DELETE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
	mov edi,eax
	.if eax==INVALID_HANDLE_VALUE
		lea eax,RLC(szErrStartup)
		APICall MessageBox,0,eax,0,MB_ICONERROR
		APICall ExitProcess,0
	.endif
	
	APICall VirtualAlloc,NULL,stEncInfo.dwOverlaySize,MEM_COMMIT,PAGE_READWRITE
	mov hMem,eax
	APICall SetFilePointer,edi,stEncInfo.dwOverlayOffset,0,FILE_BEGIN
	APICall ReadFile,edi,hMem,stEncInfo.dwOverlaySize,ADDR junk,NULL
	APICall CloseHandle,edi
	
NMET:
	lea esi,RLC(drvstr)
	mov BYTE ptr [esi],"C"
	.repeat
		inc BYTE ptr [esi]
		APICall GetDriveType,esi
		.if eax==DRIVE_REMOVABLE
			mov BYTE ptr [esi+2],0 ;convert X:\ to X:
			APICall CreateFile,ADDR RLC(devdrv),GENERIC_READ,FILE_SHARE_READ OR FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
			mov edi,eax
			.if edi==INVALID_HANDLE_VALUE
				mov BYTE ptr [esi+2],"\"
				.continue
			.endif
			mov BYTE ptr [esi+2],"\"
			push esi
			lea eax,RLC(drvstr)
			lea ecx,buff
			APICall GetDiskFreeSpaceEx,eax,0,ecx,0
			mov eax,DWORD ptr buff
			lea ecx,DWORD ptr [buff+4]
			sub eax,512
			APICall SetFilePointer,edi,eax,ecx,FILE_BEGIN
			APICall ReadFile,edi,ADDR buff,512,ADDR junk,NULL
			APICall CloseHandle,edi
			lea esi,buff
			add esi,512-SIZEOF KEYSTRUCT
			mov eax,(KEYSTRUCT ptr [esi]).dwSeed
			.if eax==stEncInfo.dwTheSeed ;BEGIN!!!!!!
				mov edi,hMem
				ProcCall TEADecrypt,edi,stEncInfo.dwOverlaySize,ADDR (KEYSTRUCT ptr [esi]).ddqTheKey
				mov esi,(PRGINFO ptr [edi]).dwSections
				mov eax,(PRGINFO ptr [edi]).dwEntryPoint
				add eax,hInstance
				movd MM0,eax
				add edi,SIZEOF PRGINFO
				;restore sections
				assume edi:ptr SECTION_INFO
				mov eax,SIZEOF SECTION_INFO
				mov edx,hMem
				mov edx,(PRGINFO ptr [edx]).dwSections
				mul edx
				add eax,SIZEOF PRGINFO
				mov junk,eax
				.while esi
					mov eax,[edi].dwRVA
					mov ecx,junk
					add eax,hInstance
					add ecx,hMem
					APICall RtlMoveMemory,eax,ecx,[edi].dwSize
					mov eax,[edi].dwSize
					add junk,eax
					dec esi
					add edi,SIZEOF SECTION_INFO
				.endw
				assume edi:nothing
				
				
				;fill original import table
				mov eax,hMem
				mov esi,(PRGINFO ptr [eax]).dwImport
				mov eax,(PRGINFO ptr [eax]).dwIsBoundImport
				mov junk,eax
				.if esi==0
					jmp heaven
				.endif
				
				add esi,hInstance
				assume esi:ptr IMAGE_IMPORT_DESCRIPTOR
				.while TRUE
					.if junk
						.break .if [esi].OriginalFirstThunk==0
					.else
						.break .if [esi].FirstThunk==0
					.endif
					mov edx,[esi].Name1
					add edx,hInstance 
					APICall LoadLibrary,edx
					mov hCurDLL,eax
					mov edi,[esi].FirstThunk
					.if junk
						mov edx,[esi].OriginalFirstThunk
					.else
						mov edx,[esi].FirstThunk
					.endif
					add edx,hInstance
					add edi,hInstance
					.while TRUE
						push edx
						.break .if DWORD PTR [edx]==0
						mov edx,[edx]
						.if edx & 80000000h ;Ordinal import
							and edx,7FFFFFFFh
						.else
							add edx,2
							add edx,hInstance
						.endif
						APICall GetProcAddress,hCurDLL,edx
						pop edx
						mov [edi],eax
						add edx,4
						add edi,4
					.endw
					pop edx
					add esi,SIZEOF IMAGE_IMPORT_DESCRIPTOR
				.endw
				jmp heaven
			.endif
			pop esi
			mov BYTE ptr [esi+2],"\"
		.endif
	.until BYTE ptr [esi]>="Z"
	lea ecx,RLC(szErrNoKeyDisk)
	APICall MessageBox,0,ecx,0,MB_OKCANCEL OR MB_ICONERROR
	cmp eax,1
	jz NMET
	
	APICall ExitProcess,0
	
heaven: 
	APICall VirtualFree,hMem,0,MEM_RELEASE
	leave
	popad
MainProc endp

movd eax,MM0
jmp eax ;fly~~~~~~~~~~~

;-----------------------------------------------------------------------------------------------------

TEADELTA equ 09E3779B9h ;(sqrt(5)-1)*2^31
TEALIMIT equ 01BBCDC80h ;DELTA*128

;TEAEncrypt proc uses ebx esi edi lpData:DWORD,cbData:DWORD,lpKey:DWORD
;	.if cbData==0
;		ret
;	.endif
;	push ebp
;	;sub esp,8
;	
;	mov eax,cbData
;	.if eax & 7
;		or eax,7
;		inc eax
;	.endif
;	add eax,lpData
;	push eax
;	push lpData
;	mov ebx,lpKey
;	.while TRUE
;		mov ecx,[esp]
;		.break .if ecx==[esp+4]
;		add DWORD ptr [esp],8
;		mov esi,[ecx]
;		mov edi,[ecx+4]
;		push ecx
;		xor edx,edx
;		.while edx!=TEALIMIT
;			mov eax,edi
;			shl eax,4
;			mov ecx,edi
;			shr ecx,5
;			xor eax,ecx
;			mov ebp,eax
;			mov eax,edi
;			xor eax,edx
;			add ebp,eax
;			mov eax,edx
;			and eax,3
;			add ebp,[ebx+4*eax]
;			add esi,ebp
;			
;			add edx,TEADELTA
;			
;			mov eax,esi
;			shl eax,4
;			mov ecx,esi
;			shr ecx,5
;			xor eax,ecx
;			mov ebp,eax
;			mov eax,esi
;			xor eax,edx
;			add ebp,eax
;			mov eax,edx
;			shr eax,11
;			and eax,3
;			add ebp,[ebx+4*eax]
;			add edi,ebp
;		.endw
;		pop ecx
;		mov [ecx],esi
;		mov [ecx+4],edi
;	.endw
;	add esp,8
;	pop ebp
;	ret
;TEAEncrypt endp
;
TEADecrypt proc uses ebx esi edi lpData:DWORD,cbData:DWORD,lpKey:DWORD
	.if cbData==0
		ret
	.endif
	push ebp
	;sub esp,8
	
	mov eax,cbData
	.if eax & 7
		or eax,7
		inc eax
	.endif
	add eax,lpData
	push eax
	push lpData
	mov ebx,lpKey
	.while TRUE
		mov ecx,[esp]
		.break .if ecx==[esp+4]
		add DWORD ptr [esp],8
		mov esi,[ecx]
		mov edi,[ecx+4]
		push ecx
		mov edx,TEALIMIT
		.while edx
			mov eax,esi
			shl eax,4
			mov ecx,esi
			shr ecx,5
			xor eax,ecx
			mov ebp,eax
			mov eax,esi
			xor eax,edx
			add ebp,eax
			mov eax,edx
			shr eax,11
			and eax,3
			add ebp,[ebx+4*eax]
			sub edi,ebp
			
			sub edx,TEADELTA
			
			mov eax,edi
			shl eax,4
			mov ecx,edi
			shr ecx,5
			xor eax,ecx
			mov ebp,eax
			mov eax,edi
			xor eax,edx
			add ebp,eax
			mov eax,edx
			and eax,3
			add ebp,[ebx+4*eax]
			sub esi,ebp
		.endw
		pop ecx
		mov [ecx],esi
		mov [ecx+4],edi
	.endw
	add esp,8
	pop ebp
	ret
TEADecrypt endp

db 13 dup(0)
db "****************"
db "*  FlashLocker *"
db "*     V1.0     *"
db "*  QQ:84065234 *"
db "*   By Proton  *"
db "*     2007     *"
db "****************"

end START