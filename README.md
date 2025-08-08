# 🧠 Malloc Implementation Project

Bu proje, C programlama dilinde dinamik bellek ayırma mekanizmasını (malloc) sıfırdan implement etmektedir.

## 📋 Proje Özellikleri

### ✅ Temel Fonksiyonlar
- `malloc(size_t size)` - Bellek ayırma
- `free(void *ptr)` - Bellek serbest bırakma
- `realloc(void *ptr, size_t size)` - Bellek yeniden boyutlandırma

### 🏗️ Mimari Tasarım
- **TINY Zone**: 1-128 byte allocationlar için optimize edilmiş
- **SMALL Zone**: 129-1024 byte allocationlar için optimize edilmiş
- **LARGE Zone**: 1024+ byte allocationlar için doğrudan mmap

### 🚀 Optimizasyonlar
- Pre-allocation stratejisi ile sistem çağrılarını minimize etme
- Block coalescing ile fragmentation önleme
- Efficient alignment (16-byte) uygulaması
- Zone-based memory management

## 🛠️ Kullanım

### Derleme
```bash
make                    # Projeyi derler
make clean             # Object dosyalarını temizler
make fclean            # Tüm derleme çıktılarını temizler
make re                # Temizler ve yeniden derler
```

### Test Etme
```bash
make test              # Test executable'ını derler
make run_test          # Testleri çalıştırır
```

### Kütüphane Kullanımı
```bash
# Program derleme
gcc program.c -L. -lft_malloc -o program

# Çalıştırma
LD_LIBRARY_PATH=. ./program
```

## 📁 Proje Yapısı

```
malloc/
├── Makefile                    # Ana build sistemi
├── includes/malloc.h           # Header dosyaları
├── src/                        # Kaynak kodlar
│   ├── malloc.c               # Ana implementation
│   ├── free.c                 # Free implementation
│   ├── realloc.c              # Realloc implementation
│   ├── zone_management.c      # Zone yönetimi
│   ├── block_management.c     # Block yönetimi
│   ├── utils.c                # Yardımcı fonksiyonlar
│   └── alignment.c            # Hizalama işlemleri
├── libft/                     # Temel kütüphane
├── test/test_malloc.c         # Test senaryoları
└── README.md                  # Dokümantasyon
```

## 🔧 Teknik Detaylar

### Memory Layout
```
Zone Structure:
[Zone Header][Block1][Data1][Block2][Data2]...[BlockN][DataN]

Block Header:
- size: allocation boyutu
- is_free: serbest durumu
- next/prev: linked list pointers
```

### Zone Sizes
- **TINY**: 16KB zonlar (~100 allocation)
- **SMALL**: 64KB zonlar (~100 allocation)
- **LARGE**: Dynamic sizing per allocation

### Alignment
- Tüm allocationlar 16-byte hizalı
- Performans ve compatibility için kritik

## 🧪 Test Senaryoları

- ✅ Basic malloc/free operations
- ✅ Realloc functionality
- ✅ Edge cases (NULL pointers, zero size)
- ✅ Memory fragmentation handling
- ✅ Large allocation management

## 📊 Performans

Bu implementation şu optimizasyonlara sahiptir:

1. **Minimal System Calls**: Pre-allocation ile mmap çağrılarını azaltma
2. **Fast Free**: O(1) free operations
3. **Coalescing**: Fragmentation'ı önlemek için block birleştirme
4. **Zone Pooling**: Sık kullanılan boyutlar için optimize edilmiş zonlar

## 🔒 Thread Safety (Bonus)

Thread-safe implementation için:
- Mutex lock'ları tüm kritik sectionlarda
- Deadlock önleme mekanizmaları
- Performance-oriented locking strategy

## 🚨 Hata Yönetimi

- Invalid pointer detection
- Double free protection
- Memory corruption checks
- Graceful degradation on errors

## 📈 İyileştirme Fırsatları

- [ ] Lock-free data structures implementation
- [ ] Memory pool optimizations
- [ ] Advanced fragmentation algorithms
- [ ] Cache-friendly memory layouts

---

**Not**: Bu implementation akademik amaçlıdır ve production kullanımı için ek testler gerekebilir.
