;; To use, run `guix build -f guix.scm` from this directory

(use-modules
 ((ice-9 match))
 ((ice-9 popen))
 ((ice-9 rdelim))
 ((srfi srfi-1))
 ((srfi srfi-26))
 ((gnu packages embedded))
 ((guix build utils) #:select (with-directory-excursion))
 ((guix build-system gnu))
 ((guix gexp))
 ((guix packages))
 ((guix utils)))

(define %source-dir (dirname (current-filename)))

(define git-file?
  (let* ((pipe (with-directory-excursion %source-dir
                 (open-pipe* OPEN_READ "git" "ls-files")))
         (files (let loop ((lines '()))
                  (match (read-line pipe)
                    ((? eof-object?)
                     (reverse lines))
                    (line
                     (loop (cons line lines))))))
         (status (close-pipe pipe)))
    (lambda (file stat)
      (match (stat:type stat)
        ('directory
         #t)
        ((or 'regular 'symlink)
         (any (cut string-suffix? <> file) files))
        (_
         #f)))))

(define (HEAD-git-version)
  (let* ((pipe (with-directory-excursion %source-dir
                 (open-pipe* OPEN_READ "git" "rev-parse" "--short=7" "HEAD")))
         (version (read-line pipe)))
    (close-pipe pipe)
    version))

(define-public biosense
  (package
    (name "biosense")
    (version (HEAD-git-version))
    (source (local-file %source-dir #:recursive? #t #:select? git-file?))
    (build-system gnu-build-system)
    (arguments
     `(#:tests? #f
       #:phases
       (modify-phases %standard-phases
         (delete 'configure)
         (replace 'build
           (lambda* (#:key (make-flags '()) inputs #:allow-other-keys)
             (let* ((toolchain (assoc-ref inputs "cross-toolchain"))
                    (cc (string-append toolchain "/bin/arm-none-eabi-gcc"))
                    (c-headers
                     (string-append
                      toolchain
                      "/arm-none-eabi/include:"
                      toolchain
                      "/arm-none-eabi/include/arm-none-eabi/armv7e-m"))
                    (c++-headers
                     (string-append
                      toolchain
                      "/arm-none-eabi/include/c++:"
                      toolchain
                      "/arm-none-eabi/include/c++/arm-none-eabi:"
                      toolchain
                      "/arm-none-eabi/include/arm-none-eabi/c++/arm-none-eabi/armv7e-m:"
                      c-headers)))
               (substitute* "nRF5_SDK_17.0.0/components/toolchain/gcc/Makefile.posix"
                 (("^GNU_INSTALL_ROOT.*")
                  (string-append toolchain "/bin/")))
               (setenv "CC" c-headers)
               (setenv "CROSS_CPATH" c-headers)
               (setenv "CROSS_CPLUS_INCLUDE_PATH" c++-headers)
               (setenv "CROSS_LIBRARY_PATH"
                       (string-append toolchain
                                      "/arm-none-eabi/lib")))
             (with-directory-excursion "gcc"
               (apply invoke "make" make-flags))
             #t))
         (delete 'check)
         (replace 'install
           (lambda* (#:key outputs #:allow-other-keys)
             (let* ((out (assoc-ref outputs "out")))
               (copy-recursively "gcc" out)
               #t))))))
    (inputs
     `(("cross-toolchain" ,arm-none-eabi-nano-toolchain-7-2018-q2-update)))
    (synopsis "")
    (home-page "")
    (description "")
    (license #f)))

biosense
