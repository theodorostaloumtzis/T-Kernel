ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Bios Parameter Block
    jmp short start
    nop

; FAT16 Header
OEMIdentifier       db 'T-KERNEL' ; OEM Identifier
BytesPerSector      dw 0x200 ; 512 bytes per sector
SectorsPerCluster   db 0x80 ; 128 sectors per cluster
ReservedSectors     dw 200 ; 200 reserved sectors
FATCopies           db 2 ; 2 FAT copies
RootDirEntries      dw 0x40 ; 64 root directory entries
NumSectors          dw 0x00 
MediaType           db 0xf8 ; Media type
SectorsPerFAT       dw 0x100 ; 256 sectors per FAT
SectorsPerTrack     dw 0x20 ; 32 sectors per track
NumberOfHeads       dw 0x40 ; 64 heads
HiddenSectors       dd 0x00 ; 0 hidden sectors
LargeSectors        dd 0x773594 

; Extended BPB (Dos 4.0)
DriveNumber        db 0x80 ; Drive number
WinNTBit           db 0x00 ; Windows NT bit
Signature          db 0x29 ; Signature
VolumeID           dd 0xd105 ; Volume ID
VolumeIDString    db 'T-KERNEL1.0' ; Volume ID String
SystemIDString    db 'FAT16   ' ; System ID String

start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enables Interrupts

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG: load32
   
;Global Descriptor Table
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:       ;   CS SHOULD POINT TO THIS   
    dw 0xffff   ;   Segment limit first 0-15 bits
    dw 0        ;   Base first 0-15 bits
    db 0        ;   Base 16-23 bits
    db 0x9a     ;   
    db 11001111b    ;   High 4 bits flags and the low 4 bit flags
    db 0        ;   Base 24-31

; offset 0x10
gdt_data:       ;   DS, SS, ES, FS, GS
    dw 0xffff   ;   Segment limit first 0-15 bits
    dw 0        ;   Base first 0-15 bits
    db 0        ;   Base 16-23 bits
    db 0x92     ;   
    db 11001111b    ;   High 4 bits flags and the low 4 bit flags
    db 0        ;   Base 24-31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1          ;   Starting Sector
    mov ecx, 100        ;   Number of Sectors to load
    mov edi, 0x0100000  ;   Address to load them
    call ata_lba_read
    jmp CODE_SEG:0x0100000


ata_lba_read:
    mov ebx, eax    ;   Backup the LBA
    ;   Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0    ;   Select the master drive
    mov dx, 0x1F6   ;   Port to read
    out dx, al
    ;   Finished sending highest 8 bits of the LBA

    ;   Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2   ;   Port to read
    out dx, al
    ;   Finished sending the total sectors to read

    ;   Send more bits of the LBA
    mov eax, ebx    ;   Restore the backup
    mov dx, 0x1F3   ;   Port to read
    out dx, al
    ;   Finished sending more bits of the LBA

    ;   Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx    ;   Restore the backup LBA
    shr eax, 8
    out dx, al
    ;   Finished sending more bits of the LBA

    ;   Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx    ;   Restore the backup LBA
    shr eax, 16
    out dx, al
    ;   Finished sending upper 16 bits of the LBA

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

    ;   Read all sectors into memory
.next_sector:
    push ecx


;   Checking  if we need to read
.try_again:
    mov dx, 0x1F7
    in al, dx
    test al, 8
    jz .try_again

;   We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw        ;   Input word from I/O port specified in DX into memory location specified in ES:(E)DI
    pop ecx
    loop .next_sector
    ;   End of reading sectors into memory
    ret 

times 510- ($-$$) db 0
dw 0xAA55       ;little endian it will read 0x55AA
