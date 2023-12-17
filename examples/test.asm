;; Example program for testing W65C02 with 6502-Teensy bridge
;; by DDRCode
;;
;; The code performs 16-bit add operation.
;; Consider that z = x+y, then
;; x is stored in $10 (hi-byte), $11 (lo-byte)
;; y is stored in $12 (hi-byte), $13 (lo-byte)
;; z is stored in $14 (hi-byte), $15 (lo-byte)
;; In the program   x=$05ff
;;                  y=$da21
;; so the result is z=$e020
;;
;; At the end of the program compares the addition with expected values.
;; Registry Y should be 0 in case of correct result and 0 otherwise.
;; Registry A should contain hi-byte of the result and X: the low-byte.
;;
;; Assembler: ACME
;; Assembling command: acme -f plain -o test.p --cpu w65c02 test.asm

; start on addres $200
* = $0200

        ; correctness indicator (1=error)
        LDY #1

        ; store $ff05 in addr $10-$11
        LDA #$05
        STA $10
        LDA #$ff
        STA $11

        ; store $da21 in addr $12-$13
        LDA #$da
        STA $12
        LDA #$21
        STA $13

        ; add lo-byte and store the result in X and addr $15
        CLC
        LDA $11
        ADC $13
        STA $15
        TAX

        ; add hi-bytes (with carry) and store the result in $14
        LDA $10
        ADC $12
        STA $14

        ; check the result and set Y to 0 if correct
        CMP #$e0
        BNE end
        CPX #$20
        BNE end
        LDY #0

end
        NOP
        NOP
        JMP end

; interrupt handling
; (although interrupts shouldn't happen in this program)
* = $0300
        NOP
        RTI


